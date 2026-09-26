#include "os.h"

 
void  OS_TickListInit (void)  //对时基列表进行初始化
{
    OS_TICK_SPOKE_IX   i;
    OS_TICK_SPOKE     *p_spoke;

    for (i = 0u; i < OSCfg_TickWheelSize; i++) {   //对OSCfg_TickWheel[]这个数组中的每一个元素中的三个数据成员，即FirstPtr、NbrEntries、NbrEntriesMax进行初始化
        p_spoke                = (OS_TICK_SPOKE *)&OSCfg_TickWheel[i];
        p_spoke->FirstPtr      = (OS_TCB        *)0;
        p_spoke->NbrEntries    = (OS_OBJ_QTY     )0u;
        p_spoke->NbrEntriesMax = (OS_OBJ_QTY     )0u;
    }
}
 
void  OS_TickListInsert (OS_TCB *p_tcb,OS_TICK time) /*当前任务要插入到时基列表中去，当前任务就是调用了阻塞延时函数OSTimeDly()的任务，即OSTCBCurPtr，这里的p_tcb就是这个
OSTCBCurPtr，在这个OS_TickListInsert()函数中就是将这个p_tcb插入到时基列表中去的，这个p_tcb阻塞延时的时钟周期数就是为time的，*/
{
	OS_TICK_SPOKE_IX   spoke;    //spoke对应时基列表中的一个数组下标 
	
	OS_TICK_SPOKE     *p_spoke;  //p_spoke指向了时基列表数组中的某一个元素
	OS_TCB            *p_tcb0;
  OS_TCB            *p_tcb1;
	
	p_tcb->TickCtrMatch = OSTickCtr + time;  //p_tcb的苏醒时刻(在一个任务中的TickCtrMatch代表了这个任务的苏醒时刻)为TickCtrMatch
	p_tcb->TickRemain   = time;              //当前任务p_tcb调用了阻塞延时函数OSTimeDly()之后需要休眠一段时间，这里的TickRemain就代表了当前任务休眠的时钟周期数
	
	spoke   = (OS_TICK_SPOKE_IX)(p_tcb->TickCtrMatch % OSCfg_TickWheelSize);  /*根据当前任务p_tcb的苏醒时刻TickCtrMatch来计算出当前任务p_tcb是要插入到时基列表数组
	OSCfg_TickWheel中的哪一个元素对应的链表中去，所得到的spoke是为时基列表数组的下标的  */
	
	p_spoke = &OSCfg_TickWheel[spoke];	//就是将当前任务p_tcb（调用了阻塞延时的任务）插入到p_soke所指向的元素对应的链表中去的

	/*
	在链表中插入结点p_tcb时分为了两种情况，当链表为空时，直接插入到链表中成为链表中的第1个结点；当链表不为空时，此时又分为了3种情况，第1种是当前结点的TickRemain比链表中所有结点的
	TickRemain都要大，此时插入到链表中的最后，第2种是当前结点比链表中的某一个结点小，如果这个结点为链表中的第1个结点的话，此时插入到链表的最开头，第3种是如果当前结点比链表中
	的某一个结点小，如果这个结点不是为链表中的第1个结点的话，插入这个结点的前面，此时插入的是两个结点之间。
	*/
	
	if (p_spoke->NbrEntries == (OS_OBJ_QTY)0u)  //1、当p_spoke对应的链表为空时，直接插入到链链表中成为链表中的第1个结点
	{
        p_tcb->TickNextPtr   = (OS_TCB   *)0;
        p_tcb->TickPrevPtr   = (OS_TCB   *)0;
        p_spoke->FirstPtr    =  p_tcb;
        p_spoke->NbrEntries  = (OS_OBJ_QTY)1u;
  }

	else	//2、当p_spoke对应的链表不为空的话，又分为了三种情况 
	{		
		p_tcb1 = p_spoke->FirstPtr;	//p_tcb1依次指向了链表中的每一个结点
		
		while (p_tcb1 != (OS_TCB *)0)
		{
			
			p_tcb1->TickRemain = p_tcb1->TickCtrMatch - OSTickCtr; //依次计算出当前任务p_tcb要插入的那个链表中的每一个结点的休眠时间（用苏醒的时刻减去现在的时刻就是为休眠的时间的）
                               
           
			if (p_tcb->TickRemain > p_tcb1->TickRemain)  //当前任务的休眠时间大于进行比较的那一个结点的休眠时间的话就接着往下找（是按照休眠时间从小到大的方式来进行排列的）
			{        
          
				 if (p_tcb1->TickNextPtr != (OS_TCB *)0)   //进行比较的结点不是为链表中的最后一个结点的话         
				    	p_tcb1 =  p_tcb1->TickNextPtr;       //p_tcb1指向了链表中的下一个结点
				 else	//<1>进行比较的结点为链表中的最后一个结点时，对应了第1种情况，即是当前任务p_tcb的休眠时间比链表中所有任务的休眠时间都要大
				 {
                    p_tcb->TickNextPtr   = (OS_TCB *)0;
                    p_tcb->TickPrevPtr   =  p_tcb1;
                    p_tcb1->TickNextPtr  =  p_tcb;               
                    p_tcb1               = (OS_TCB *)0;
          }
       }	
			else	//说明了当前任务p_tcb的休眠时间比要进行比较的那个结点的休眠时间小，此时应该插入到这个结点的前面
			{                                             
             
				if (p_tcb1->TickPrevPtr == (OS_TCB *)0)    //<2>如果p_tcb1为链表中的第1个结点的话，对应了第2种情况
				{
                    p_tcb->TickPrevPtr   = (OS_TCB *)0;
                    p_tcb->TickNextPtr   =  p_tcb1;
                    p_tcb1->TickPrevPtr  =  p_tcb;
                    p_spoke->FirstPtr    =  p_tcb;
        } 
				else   //<3>如果p_tcb1不是为链表中的第1个结点的话，此时应该插入到两个节点之间，对应了第3种情况
				{                                    
                    p_tcb0               =  p_tcb1->TickPrevPtr;
                    p_tcb->TickPrevPtr   =  p_tcb0;
                    p_tcb->TickNextPtr   =  p_tcb1;
                    p_tcb0->TickNextPtr  =  p_tcb;
                    p_tcb1->TickPrevPtr  =  p_tcb;
        }
				
         p_tcb1 = (OS_TCB *)0;   //执行完上面的操作后说明此时已经将当前任务p_tcb（当前任务p_tcb调用延时函数OSTimeDly()被阻塞了）插入到时基列表中了，此时应该跳出while循环 
     }
		}//while循环的
		p_spoke->NbrEntries++; //结点成功插入则记录最大结点数的NbEntries应该加1
	}//最外层的else的
	
	
	if (p_spoke->NbrEntriesMax < p_spoke->NbrEntries)		// 刷新NbrEntriesMax的值
	      p_spoke->NbrEntriesMax = p_spoke->NbrEntries;
	p_tcb->TickSpokePtr = p_spoke;	//当前任务p_tcb中有一个成员指向了根结点
}




void  OS_TickListRemove (OS_TCB  *p_tcb)   //从时基列表中移除一个任务 
{
	OS_TICK_SPOKE  *p_spoke;
    OS_TCB         *p_tcb1;
    OS_TCB         *p_tcb2;
	
	
	p_spoke = p_tcb->TickSpokePtr;       //获取任务TCB所在链表的根指针
	
	
	if (p_spoke != (OS_TICK_SPOKE *)0)   //确保任务在链表中 
	{                              
       
		p_tcb->TickRemain = (OS_TICK)0u;  //将剩余时间清0
		
		
     if (p_spoke->FirstPtr == p_tcb)  //要移除的刚好是第一个节点 
		 {                             
			 p_tcb1=(OS_TCB *)p_tcb->TickNextPtr;   //更新第一个节点，原来的第一个节点需要被移除 
       p_spoke->FirstPtr = p_tcb1;
       if (p_tcb1 != (OS_TCB *)0) 
            p_tcb1->TickPrevPtr = (OS_TCB *)0;
            
      }
			
		else 	  //要移除的不是第一个节点
		{
            
			  p_tcb1              = p_tcb->TickPrevPtr;    //保存要移除的节点的前后节点的指针
        p_tcb2              = p_tcb->TickNextPtr;
        p_tcb1->TickNextPtr = p_tcb2;	               //节点移除，将节点前后的两个节点连接在一起 
        if (p_tcb2 != (OS_TCB *)0) 
			       p_tcb2->TickPrevPtr = p_tcb1;
    }
		
        p_tcb->TickNextPtr  = (OS_TCB        *)0;	   //复位任务TCB中时基列表相关的字段成员 
        p_tcb->TickPrevPtr  = (OS_TCB        *)0;
        p_tcb->TickSpokePtr = (OS_TICK_SPOKE *)0;
        p_tcb->TickCtrMatch = (OS_TICK        )0u;
		
        p_spoke->NbrEntries--;	//节点减1
    }
}


void  OS_TickListUpdate (void) /*对时基列表进行更新，由于这个函数是写在时钟中断的中断服务函数OSTimeTick()里面的，故是每触发一次时钟中断就会执行一次这个函数，由于时钟中断
设置的是每10ms触发一次，故这里是为每隔10ms来对时基列表进行一次更新。 */
{
	OS_TICK_SPOKE_IX   spoke;
	OS_TICK_SPOKE     *p_spoke;
	OS_TCB            *p_tcb;
	OS_TCB            *p_tcb_next;
	CPU_BOOLEAN        done;
	
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();  //进入临界段 
	
	OSTickCtr++;          //时基计数器++ ，时钟中断每触发一次就会执行一次这个函数，故是每触发一次时钟中断就会对时基计数器进行加1的操作
	
	spoke    = (OS_TICK_SPOKE_IX)(OSTickCtr % OSCfg_TickWheelSize);  //计算出当前时刻OSTickCtr(当前时刻OSTickCtr是为一个全局变量的)所对应的时基列表数组的下标
	p_spoke  = &OSCfg_TickWheel[spoke];   //p_spoke指向了当前时刻所对应的时基列表数组中的那一个元素
	
	p_tcb    = p_spoke->FirstPtr;  //p_tcb指向了当前时刻所对应的那一个列表（每一个时刻都是可以对OSCfg_TickWheelSize进行求余操作得到一个下标的，故每一个时刻都对应了一个链表）
	done     = DEF_FALSE;
	
	while (done == DEF_FALSE)
	{
		if (p_tcb != (OS_TCB *)0) 
		{
			p_tcb_next = p_tcb->TickNextPtr; //p_tcb_next指向了p_tcb所指向的结点的下一个结点
			
			p_tcb->TickRemain = p_tcb->TickCtrMatch - OSTickCtr; //这一步是多余的
			
			if (OSTickCtr == p_tcb->TickCtrMatch)		//p_tcb所指向的结点的苏醒时刻等于当前的时刻的话
				OS_TaskRdy(p_tcb);	/*这个函数调用了两个函数,分别是为时基列表删除函数和就绪列表插入函数，这个函数所进行的操作就是从当前时基列表中删除这个p_tcb,同时将和这个p_tcb加入
			到就绪列表中去（注意，在就绪列表的插入操作中是有对要插入的结点所对应的优先级表中的位进行置为1的操作的）*/
			else
			{
				/* 如果第一个节点延时期未满，则退出while循环
				   因为链表是根据升序排列的，第一个节点延时期未满，那后面的肯定未满 */
				done = DEF_TRUE;	
			}		

			/* 如果第一个节点延时期满，则继续遍历链表，看看还有没有延时期满的任务
			   如果有，则让它就绪 */
			p_tcb = p_tcb_next;
		}
		else
		{
			done  = DEF_TRUE; 
		}
	}
	
	OS_CRITICAL_EXIT();  //退出临界段 
}
