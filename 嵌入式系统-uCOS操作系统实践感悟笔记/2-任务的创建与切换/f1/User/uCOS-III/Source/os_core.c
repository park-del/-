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
	OSRunning =  OS_STATE_OS_STOPPED;
	
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	OS_RdyListInit();
	
	*p_err = OS_ERR_NONE;
}

/* 启动RTOS，将不再返回 */
void OSStart (OS_ERR *p_err)
{	
	if( OSRunning == OS_STATE_OS_STOPPED )  //OSRunning在操作系统初始化时已经被赋值为OS_STATE_OS_STOPPED了，此时如果操作系统已经完成了初始化就执行if语句里面的内容
	{
	 
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;  //指明操作系统启动时要运行哪个任务
	
		OSStartHighRdy();    //使操作系统指明的那一个任务去执行其所对应的那一个函数
		
		/* 不会运行到这里，运行到这里表示发生了致命的错误 */
		*p_err = OS_ERR_FATAL_RETURN;
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}

/* 任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换 */
void OSSched_1 (void)  //由1号任务切换到2号任务
{
	
	if( OSTCBCurPtr == OSRdyList[0].HeadPtr )  //如果当前运行的是就绪队列中的0号任务，即是Task1TCB的话，就执行这个if语句里面的内容
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;  //切换到就绪队列中的1号任务，即是Task2TCB任务去执行
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
	
	OS_TASK_SW();      //进行任务的切换，其中OSTCBCurPtr指针指向的是当前正在运行的那个线程，OSTCBHighRdyPtr指针指向要切换的那个线程，在进行线程的切换时要进行对OSTCBCurPtr
}  //所指向的那个线程的现场环境的保存（将OSTCBCurPtr所指向的这个线程的现场环境的信息压入到这个线程对应的任务堆栈中），要进行对OSTCBHighRdyPtr所指向的那个线程的现场环境的恢复
   //（要取出OSTCBHighTdyPtr所指向的那个线程所对应的任务堆栈中的栈顶指针所指向的空间里面的现场环境的一些信息，对相关寄存器进行赋值）。

void OSSched_2 (void) //由2号任务切换到3号任务
{
	
	if( OSTCBCurPtr == OSRdyList[1].HeadPtr )
	{
		OSTCBHighRdyPtr = OSRdyList[2].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
	}
	
	OS_TASK_SW();
}

void OSSched_3 (void) //由3号任务切换到1号任务
{
	
	if( OSTCBCurPtr == OSRdyList[2].HeadPtr )
	{
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	}
	else
	{
		OSTCBHighRdyPtr = OSRdyList[2].HeadPtr;
	}
	
	OS_TASK_SW();
}
