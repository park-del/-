#include "os.h"

/* 就绪列表初始化 */
void OS_RdyListInit(void)
{
	OS_PRIO i;
	OS_RDY_LIST *p_rdy_list;
	
	for( i=0u; i<OS_CFG_PRIO_MAX; i++ )
	{
		p_rdy_list = &OSRdyList[i];
		p_rdy_list->HeadPtr = (OS_TCB *)0;
		p_rdy_list->TailPtr = (OS_TCB *)0;
	}
}

/* RTOS初始化
** 初始化全局变量
*/
void OSInit (OS_ERR *p_err)
{
	/* 配置OS初始状态为停止态 */
	OSRunning =  OS_STATE_OS_STOPPED;
	
	/* 初始化两个全局TCB，这两个TCB用于任务切换 */
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	/* 初始化就绪列表 */
	OS_RdyListInit();
	
	/* 初始化空闲任务 */
	OS_IdleTaskInit(p_err);
	if (*p_err != OS_ERR_NONE) 
	{
        return;
    }
}

/* 启动RTOS，将不再返回 */
void OSStart (OS_ERR *p_err)
{	
	if( OSRunning == OS_STATE_OS_STOPPED )
	{
		/* 手动配置任务1先运行 */
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		
		/* 启动任务切换，不会返回 */
		OSStartHighRdy();
		
		/* 不会运行到这里，运行到这里表示发生了致命的错误 */
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}

/* 任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换 */
void OSSched(void)
{
#if 0	//非常简单的任务调度：两个任务轮流执行 
	if( OSTCBCurPtr == OSRdyList[0].HeadPtr )
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
#endif
	
	/* 如果当前任务是空闲任务，那么就去尝试执行任务1或者任务2，看看他们的延时时间是否结束
	   如果任务的延时时间均没有到期，那就返回继续执行空闲任务 */
	
	if( OSTCBCurPtr == &OSIdleTaskTCB ) //如果当前任务是为空闲任务的话，就去查看任务列表中是否有一个任务的延时周期数为0，如果为0的话就切换到这个任务去执行（是按照各个任务在列表中的顺序来界定优先级的，
		//即是在这个任务列表中如果同时有两个任务的延时周期数为0的话优先切换的任务列表中前面的那个任务。如果任务列表中没有任务的周期数为0的话就直接return ，不用在执行OSSched()函数后面的语句了）
	{
		if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
		{
			OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
		}
		else if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
		{
			OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
		}
		else
		{
			return;		/* 任务延时均没有到期则返回，继续执行空闲任务 */
		} 
	}
	else
	{
		/*如果是task1或者task2的话，检查下另外一个任务,如果另外的任务不在延时中，就切换到该任务
        否则，判断下当前任务是否应该进入延时状态，如果是的话，就切换到空闲任务。否则就不进行任何切换 */
		
		
		//
		if(OSTCBCurPtr == OSRdyList[0].HeadPtr) //如果当前任务是为任务1的话，查看任务2的延时周期数是否为0，如果是的话就切换到任务2。当前任务是任务1说明了任务1调用了OSSched()函数，
		{//而这个函数又是在OSTimeDly(2);这个函数中定义的，说民了任务1调用了这个函数，任务1要进行阻塞延时。如果任务列表中没有一个任务的延时周期数为0，而且自己的延时周期数也不是为0的话，
			//那么此时就只能够切换到空闲任务去执行了。
			if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
			{
				OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;
			}
			else 
			{
				return;		/* 返回，不进行切换，因为两个任务都处于延时中 */
			}
		}
		else if(OSTCBCurPtr == OSRdyList[1].HeadPtr) //如果当前任务是任务2的话也是一样的，看任务列表中哪一个任务的时钟周期数为0就切换到哪一个任务去执行。排在任务列表前面的任务是优先切换的，这也是可以人为的进行修改的，先判断哪一个则哪一个的优先级就会高。 
		{
			if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
			{
				OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
			}
			else if(OSTCBCurPtr->TaskDelayTicks != 0)
			{
				OSTCBHighRdyPtr = &OSIdleTaskTCB;
			}
			else 
			{
				return;		/* 返回，不进行切换，因为两个任务都处于延时中 */
			}
		}
	}
	
	
	OS_TASK_SW();/*执行任务切换函数，如果前面return了的话执行这个OSSched()函数后就不会执行这个OS_TASK_SW()函数了。即是当前任务为空闲任务，而且任务列表中又没有一个任务的时钟周期数为0的话。
	如果执行这个OSSched()函数的任务为任务1的话（说明了这个任务1是要进行阻塞的，这个任务1执行了阻塞函数	OSTimeDly(2)，先按照顺序判断出任务列表中的所有任务的延时周期数都不为0
	（按照任务列表的顺序隐藏了一种从当前这个任务1切换到另一个任务的优先级的），只有任务列表中的所有任务的任务周期数都不为0才会切换到空闲任务，因此这里切换到空闲任务的语句应该
	放在判断完任务列表中所有的任务之后。）*/	
	
	//注意：切换到空闲任务的时候还是要执行这个OS_TASK_SW()函数的，因为只有执行了这个函数之后才能够进行真正的任务切换
}


/* 空闲任务 */
void  OS_IdleTask (void  *p_arg)  //如果要切换的是空闲任务，当执行完这个OS_TASK_SW();函数之后就会cpu就会切换到这个空闲任务来去执行。注意：执行这个空闲任务的时候也是会触发时钟中断的
{ //，执行时钟中断的中断服务函数时会使任务列表中所有任务的时钟周期数减1，然后会根据任务列表中的各个任务的时钟周期数来决定要切换到哪一个任务去执行（由于进行任务切换时有进行保存一个
	//任务现场的工作，故切换任务时返回的是原来任务的被阻塞的地方的，从这个被阻塞的地方接着执行的）
	p_arg = p_arg;
	
	/* 空闲任务什么都不做，只对全局变量OSIdleTaskCtr ++ 操作 */
	for(;;)
	{
		OSIdleTaskCtr++;
	}
}

/* 空闲任务初始化 */
void  OS_IdleTaskInit(OS_ERR  *p_err)
{	
	
	OSIdleTaskCtr = (OS_IDLE_CTR)0;/* 初始化空闲任务计数器，给这个计数器赋初值为0 */
	
	/* 创建空闲任务 */
	OSTaskCreate( (OS_TCB     *)&OSIdleTaskTCB, 
			      (OS_TASK_PTR )OS_IdleTask, 
			      (void       *)0,
			      (CPU_STK    *)OSCfg_IdleTaskStkBasePtr,
			      (CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
			      (OS_ERR     *)p_err );
}



