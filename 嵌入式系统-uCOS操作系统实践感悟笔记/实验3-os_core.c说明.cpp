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
	OSRunning =  OS_STATE_OS_STOPPED;   //操作系统当前的运行状态为OS_STATE_OS_STOPPED时表明操作系统当前的运行状态是为初始状态的
	
	OSTCBCurPtr = (OS_TCB *)0;
	OSTCBHighRdyPtr = (OS_TCB *)0;
	
	OS_RdyListInit();
	
	*p_err = OS_ERR_NONE;
}

/* 启动RTOS，将不再返回 */
void OSStart (OS_ERR *p_err)
{	
	if( OSRunning == OS_STATE_OS_STOPPED )     //如果操作系统当前运行的状态为初始状态的话，即是没有运行任务时，此时启动操作系统后就要进行配置要运行那一个任务
	{
		 
		OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;  /*让任务1线运行（位于就绪队列中的0号空间中的是为任务1的），其中OSTCBHighRdyPtr指向要进行切换的任务（从其英文单词中就很容易知道
	  其表示的是什么意思了，OS表示的是操作系统，TCB表示的是某一个任务，HighRdy表示准备运行，Ptr是指针的意思，即是OSTCBHighRdyPtr就是一个指向要进行切换的任务的指针）    */
		
		 
		OSStartHighRdy();  //执行任务切换
		
		*p_err = OS_ERR_FATAL_RETURN;/* 不会运行到这里，运行到这里表示发生了致命的错误 */
	}
	else
	{
		*p_err = OS_STATE_OS_RUNNING;
	}
}

/* 任务切换，实际就是触发PendSV异常，然后在PendSV异常中进行上下文切换 */
void OSSched (void)
{
	if( OSTCBCurPtr == OSRdyList[0].HeadPtr ) //OS表示操作系统，TCB表示一个TCB，Cur表示当前的意思，Ptr表示指针的意思，则OSTCBCurPtr就是指向当前正在占用CPU的一个任务的指针
	{
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
		/*
		if(OSTCBCurPtr==OSRdyList[0].HeadPtr)表示的意思就是如果当前正在运行的任务为Task1的话，if(OSTCBCurPtr==OSRdyList[1].HeadPtr)表示的意思是如果当前正在运行的任务为Task2的话。
		OSTCBHighRdyPtr = OSRdyList[1].HeadPtr表示切换到任务2去运行，OSTCBHighRdyPtr = OSRdyList[0].HeadPtr表示切换到任务1去运行。
		*/
	}
	else
	    if( OSTCBCurPtr == OSRdyList[1].HeadPtr )
		  {
		    OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	    }
				 else
	       {
		       OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
	       }
	
  OS_TASK_SW();/*OS_TASK_SW()就是进行任务切换的任务的，将OSTCBCurPtr指针所指向的当前任务的现场环境压入到这个任务所对应的任务堆栈中去，即是保存当前任务的现场;然后从
	OSTCBHighRdyPtr所指向的任务的任务堆栈中的栈顶出取出这个任务所对应的现场环境的信息，进行恢复这个任务的现场。			 */
}


