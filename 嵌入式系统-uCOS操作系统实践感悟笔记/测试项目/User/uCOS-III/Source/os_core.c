#include "os.h"



/* RTOS初始化
** 初始化全局变量
*/
void OSInit (OS_ERR *p_err)
{
	/* 配置OS初始状态为停止态 */
	OSRunning =  OS_STATE_OS_STOPPED;
	
	 
	OSTCBCurPtr = (OS_TCB *)0;        //OSTCBCurPtr指向了当前任务
	OSTCBHighRdyPtr = (OS_TCB *)0;    //OSTCBHighRdyPtr指向了要切换的那个任务
	
	OSPrioCur                       = (OS_PRIO)0;    //OSPrioCur指向了当前任务的优先级（这个数值是为1、2、3、4的，表示的是优先级）       
  OSPrioHighRdy                   = (OS_PRIO)0;    //OSPrioHighRdy指向了任务列表中最高优先级的那个任务
	 
	OS_PrioInit();       //初始的时候对优先级表进行了初始化
	
	OS_RdyListInit();    //初始化就绪列表
	
	/* 初始化空闲任务 */
	OS_IdleTaskInit(p_err);
	if (*p_err != OS_ERR_NONE) 
	{
        return;
    }
}

 
void OSStart (OS_ERR *p_err) //启动操作系统
{	
	if( OSRunning == OS_STATE_OS_STOPPED )
	{
 
		OSPrioHighRdy   = OS_PrioGetHighest();  //获取优先级表中最高的优先级，即是对优先级表从前往后遍历，找到第1个为1的，这个第1个为1的所表示的序号就是为这个序号所对应的优先级
		OSPrioCur       = OSPrioHighRdy;        //将当前优先级修改为最高的这个优先级
		
		OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;/*OSPrioHighRdy为最高优先级，最高优先级对应就是为就绪列表的数组下标的，此时数组中这个序号的元素所对应的链
		表中的每一个结点就是一个该优先级的任务。（这里获得的是为最高优先级的TCB的，OSTCBHighRdyPtr指针时指向最高优先级的那个任务的TCB的）*/
		OSTCBCurPtr     = OSTCBHighRdyPtr;      //当前任务指向最高优先级的那个任务
		
		OSRunning       = OS_STATE_OS_RUNNING;  //标记OS开始运行 
		
		
		OSStartHighRdy(); //启动任务切换，不会返回 
		
		/* 不会运行到这里，运行到这里表示发生了致命的错误 */
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}


void OSSched(void)    //任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换
{
 
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();	//进入临界区 
	OSPrioHighRdy   = OS_PrioGetHighest(); //这里获得是为最高优先级的
	OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;	  //OSTCBHighRdyPtr指向了最高优先级的那个任务的TCB
	
	
	if (OSTCBHighRdyPtr == OSTCBCurPtr)    //如果最高优先级的任务是当前任务则直接返回，此时是不需要进行任务切换的 
	{
		
		OS_CRITICAL_EXIT();  //退出临界区 
		
		return;
	}	
	
	OS_CRITICAL_EXIT();	   //退出临界区 
	
	
	OS_TASK_SW();	//任务切换,要切换的任务就是 OSTCBHighRdyPtr 指针所指向的那个任务，由于OSTCBHighRdyPtr所指向的那个任务是为最高优先级的那个任务，故这里切换的是为最高优先级的那个任务。
}



void  OS_IdleTask (void  *p_arg)  //空闲任务
{
	p_arg = p_arg;
	
	for(;;)	    //空闲任务什么都不做，只对全局变量OSIdleTaskCtr ++ 操作 
	{
		OSIdleTaskCtr++;
	}
}
 
/* 空闲任务初始化 */
void  OS_IdleTaskInit(OS_ERR  *p_err)
{	
	/* 初始化空闲任务计数器 */
	OSIdleTaskCtr = (OS_IDLE_CTR)0;
	
	/* 创建空闲任务 */
	OSTaskCreate( (OS_TCB     *)&OSIdleTaskTCB, 
			      (OS_TASK_PTR )OS_IdleTask, 
			      (void       *)0,
				  (OS_PRIO)(OS_CFG_PRIO_MAX - 1u),
			      (CPU_STK    *)OSCfg_IdleTaskStkBasePtr,
			      (CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
			      (OS_ERR     *)p_err );
}

/*
*********************************************************************************************************
*                                           就绪列表相关函数
*********************************************************************************************************
*/
/* 就绪列表初始化 */
/*
*              'OS_RDY_LIST  OSRdyTbl[OS_CFG_PRIO_MAX]'  初始化之后就像下面这样：
*
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*                          [0]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*                          [1]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
*                                       :              :
*                                       :              :
*                                       :              :
*                               +---------------+--------------+
*                               |               | TailPtr      |-----> 0
*          [OS_CFG_PRIO_MAX-1]  | NbrEntries=0  +--------------+
*                               |               | HeadPtr      |-----> 0
*                               +---------------+--------------+
   注意：空闲任务是为最低优先级所对应的那个链表中的某一个结点的，如果是为32个优先级的话则空闲任务所对应的就是为第31号优先级所对应的

*/
void OS_RdyListInit(void)
{
	OS_PRIO i;
	OS_RDY_LIST *p_rdy_list;
	
	for( i=0u; i<OS_CFG_PRIO_MAX; i++ )
	{
		p_rdy_list = &OSRdyList[i];
		p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
		p_rdy_list->HeadPtr = (OS_TCB *)0;
		p_rdy_list->TailPtr = (OS_TCB *)0;
	}
}


/* 在就绪链表中插入一个TCB */
void  OS_RdyListInsert (OS_TCB  *p_tcb)
{
    /* 将优先级插入到优先级表 */
	OS_PrioInsert(p_tcb->Prio);
	
    if (p_tcb->Prio == OSPrioCur) 
	{
        /* 如果是当前优先级则插入到链表尾部 */
		OS_RdyListInsertTail(p_tcb);  //这里是要进行时间片的轮转的，当前任务的优先级与要切换的任务那个任务的优先级是一样的话则当前任务插入到这个优先级所对应的链表的尾部（要进行事件片的轮转）
    } 
	else 
	{
        /* 否则插入到链表头部 */
		OS_RdyListInsertHead(p_tcb);
    }
	/*
		进行切换的时候所切换的是为某一个优先级所对应的链表中的第1个结点的，如果为其他的优先级的话则插入到这个优先级所对应的链表的头部，表示下次切换这个优先级的任务时所切换的是为
		这个任务。（所切换的是为这个优先级所对应的链表中的第1个结点的 ）
	*/
		
}

/* 在就绪链表头部插入一个TCB */

	/*
	*  在链表头部插入一个TCB有以下两种情况
	*              CASE 0: 插入到一个空的链表
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-> 0
	*                     +--------------+
	*                     | HeadPtr      |-> 0
	*                     +--------------+
	*                     | NbrEntries=0 |
	*                     +--------------+
	*
	*
	*
	*              CASE 1: 链表已经有节点
	*
	*                     OS_RDY_LIST
	*                     +--------------+          OS_TCB
	*                     | TailPtr      |--+---> +------------+
	*                     +--------------+  |     | NextPtr    |->0
	*                     | HeadPtr      |--/     +------------+
	*                     +--------------+     0<-| PrevPtr    |
	*                     | NbrEntries=1 |        +------------+
	*                     +--------------+        :            :
	*                                             :            :
	*                                             +------------+
	*
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-----------------------------------------------+
	*                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
	*                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
	*                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
	*                     | NbrEntries=N |        +------------+       +------------+        +------------+
	*                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
	*                                             +------------+       +------------+        +------------+
	*                                             :            :       :            :        :            :
	*                                             :            :       :            :        :            :
	*                                             +------------+       +------------+        +------------+
	*/
	
void  OS_RdyListInsertHead (OS_TCB  *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb2;



    /* 获取链表根部 */
	p_rdy_list = &OSRdyList[p_tcb->Prio];
	
	/* CASE 0: 链表是空链表 */
    if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) 
	{
		p_rdy_list->NbrEntries =  (OS_OBJ_QTY)1;            
        p_tcb->NextPtr         =  (OS_TCB   *)0;            
        p_tcb->PrevPtr         =  (OS_TCB   *)0;
        p_rdy_list->HeadPtr    =  p_tcb;                    
        p_rdy_list->TailPtr    =  p_tcb;
    }
	/* CASE 1: 链表已有节点 */	
	else 
	{                                               
        p_rdy_list->NbrEntries++;                           
        p_tcb->NextPtr         = p_rdy_list->HeadPtr;       
        p_tcb->PrevPtr         = (OS_TCB    *)0;
        p_tcb2                 = p_rdy_list->HeadPtr;
        p_tcb2->PrevPtr        = p_tcb;
        p_rdy_list->HeadPtr    = p_tcb;
    }
}


/* 在就绪链表尾部插入一个TCB */

	/*
	*  在就绪链表尾部插入一个TCB 有以下 两种情况
	*              CASE 0: 链表是空的
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-> 0
	*                     +--------------+
	*                     | HeadPtr      |-> 0
	*                     +--------------+
	*                     | NbrEntries=0 |
	*                     +--------------+
	*
	*
	*
	*              CASE 1: 链表已有节点
	*
	*                     OS_RDY_LIST
	*                     +--------------+          OS_TCB
	*                     | TailPtr      |--+---> +------------+
	*                     +--------------+  |     | NextPtr    |->0
	*                     | HeadPtr      |--/     +------------+
	*                     +--------------+     0<-| PrevPtr    |
	*                     | NbrEntries=1 |        +------------+
	*                     +--------------+        :            :
	*                                             :            :
	*                                             +------------+
	*
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-----------------------------------------------+
	*                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
	*                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
	*                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
	*                     | NbrEntries=N |        +------------+       +------------+        +------------+
	*                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
	*                                             +------------+       +------------+        +------------+
	*                                             :            :       :            :        :            :
	*                                             :            :       :            :        :            :
	*                                             +------------+       +------------+        +------------+
	*/
	
void  OS_RdyListInsertTail (OS_TCB  *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb2;


    /* 获取链表根部 */
    p_rdy_list = &OSRdyList[p_tcb->Prio];
	
	/* CASE 0: 链表是空链表 */
    if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) 
	{          
        p_rdy_list->NbrEntries  = (OS_OBJ_QTY)1;            
        p_tcb->NextPtr          = (OS_TCB   *)0;           
        p_tcb->PrevPtr          = (OS_TCB   *)0;
        p_rdy_list->HeadPtr     = p_tcb;                   
        p_rdy_list->TailPtr     = p_tcb;
    } 
	/* CASE 1: 链表已有节点 */
	else 
	{                                               
        p_rdy_list->NbrEntries++;                           
        p_tcb->NextPtr          = (OS_TCB   *)0;            
        p_tcb2                  = p_rdy_list->TailPtr;
        p_tcb->PrevPtr          = p_tcb2;
        p_tcb2->NextPtr         = p_tcb;                   
        p_rdy_list->TailPtr     = p_tcb;
    }
}


/* 将链表中的节点从头部移动到尾部 */

	/*
	*  将链表中的节点从头部移动到尾部，包含下面三种情况：
	*              CASE 0: 链表为空，无事可做。
	*
	*              CASE 1: 链表只有一个节点，无事可做。
	*
	*              CASE 2: 链表只有两个节点。
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |--------------------------+
	*                     +--------------+          OS_TCB          |     OS_TCB
	*                     | HeadPtr      |------> +------------+    +-> +------------+
	*                     +--------------+        | NextPtr    |------> | NextPtr    |->0
	*                     | NbrEntries=2 |        +------------+        +------------+
	*                     +--------------+     0<-| PrevPtr    | <------| PrevPtr    |
	*                                             +------------+        +------------+
	*                                             :            :        :            :
	*                                             :            :        :            :
	*                                             +------------+        +------------+
	*
	*
	*              CASE N: 链表有两个节点以上。
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-----------------------------------------------+
	*                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
	*                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
	*                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
	*                     | NbrEntries=N |        +------------+       +------------+        +------------+
	*                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
	*                                             +------------+       +------------+        +------------+
	*                                             :            :       :            :        :            :
	*                                             :            :       :            :        :            :
	*                                             +------------+       +------------+        +------------+
	*/
	
void  OS_RdyListMoveHeadToTail (OS_RDY_LIST  *p_rdy_list)
{
    OS_TCB  *p_tcb1;
    OS_TCB  *p_tcb2;
    OS_TCB  *p_tcb3;



    switch (p_rdy_list->NbrEntries) 
	{
        case 0:
        case 1:
             break;

        case 2:                                             
             p_tcb1              = p_rdy_list->HeadPtr;    
             p_tcb2              = p_rdy_list->TailPtr;    
             p_tcb1->PrevPtr     = p_tcb2;
             p_tcb1->NextPtr     = (OS_TCB *)0;
             p_tcb2->PrevPtr     = (OS_TCB *)0;
             p_tcb2->NextPtr     = p_tcb1;
             p_rdy_list->HeadPtr = p_tcb2;
             p_rdy_list->TailPtr = p_tcb1;
             break;

        default:                                            
             p_tcb1              = p_rdy_list->HeadPtr;     
             p_tcb2              = p_rdy_list->TailPtr;     
             p_tcb3              = p_tcb1->NextPtr;         
             p_tcb3->PrevPtr     = (OS_TCB *)0;            
             p_tcb1->NextPtr     = (OS_TCB *)0;           
             p_tcb1->PrevPtr     = p_tcb2;                  
             p_tcb2->NextPtr     = p_tcb1;                 
             p_rdy_list->HeadPtr = p_tcb3;                  
             p_rdy_list->TailPtr = p_tcb1;
             break;
    }
}

/* 从链表中移除一个TCB */

	/*
	*  从链表中移除一个TCB有以下几种情况：
	*              CASE 0: 链表为空，无事可做。
	*
	*              CASE 1: 链表只有一个节点。
	*
	*                     OS_RDY_LIST
	*                     +--------------+          OS_TCB
	*                     | TailPtr      |--+---> +------------+
	*                     +--------------+  |     | NextPtr    |->0
	*                     | HeadPtr      |--/     +------------+
	*                     +--------------+     0<-| PrevPtr    |
	*                     | NbrEntries=1 |        +------------+
	*                     +--------------+        :            :
	*                                             :            :
	*                                             +------------+
	*
	*              CASE N: 链表中有两个以上的节点。
	*
	*                     OS_RDY_LIST
	*                     +--------------+
	*                     | TailPtr      |-----------------------------------------------+
	*                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
	*                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
	*                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
	*                     | NbrEntries=N |        +------------+       +------------+        +------------+
	*                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
	*                                             +------------+       +------------+        +------------+
	*                                             :            :       :            :        :            :
	*                                             :            :       :            :        :            :
	*                                             +------------+       +------------+        +------------+
	*/

void  OS_RdyListRemove (OS_TCB  *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb1;
    OS_TCB       *p_tcb2;



    p_rdy_list = &OSRdyList[p_tcb->Prio];
	
	/* 保存要删除的TCB节点的前一个和后一个节点 */
    p_tcb1     = p_tcb->PrevPtr;
    p_tcb2     = p_tcb->NextPtr;
	
	/* 要移除的TCB节点是链表中的第一个节点 */
    if (p_tcb1 == (OS_TCB *)0) 
	{                            
        /* 且该链表中只有一个节点 */
		if (p_tcb2 == (OS_TCB *)0) 
		{                       
            /* 根节点全部初始化为0 */
			p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
            p_rdy_list->HeadPtr    = (OS_TCB   *)0;
            p_rdy_list->TailPtr    = (OS_TCB   *)0;
			
            /* 清除在优先级表中相应的位 */
			OS_PrioRemove(p_tcb->Prio);
        }
		/* 该链表中不止一个节点 */		
		else 
		{
            /* 节点减1 */
			p_rdy_list->NbrEntries--;
            p_tcb2->PrevPtr        = (OS_TCB   *)0;        
            p_rdy_list->HeadPtr    = p_tcb2; 
        }
    }
	/* 要移除的TCB节点不是链表中的第一个节点 */	
	else 
	{
        p_rdy_list->NbrEntries--;                          
        p_tcb1->NextPtr = p_tcb2;
		
        /* 如果要删除的节点的下一个节点是0，即要删除的节点是最后一个节点 */
		if (p_tcb2 == (OS_TCB *)0) 
		{
            p_rdy_list->TailPtr = p_tcb1;
        } 
		else 
		{
            p_tcb2->PrevPtr     = p_tcb1;
        }
    }
	
	/* 复位从就绪列表中删除的TCB的PrevPtr和NextPtr这两个指针 */
    p_tcb->PrevPtr = (OS_TCB *)0;
    p_tcb->NextPtr = (OS_TCB *)0;
}










