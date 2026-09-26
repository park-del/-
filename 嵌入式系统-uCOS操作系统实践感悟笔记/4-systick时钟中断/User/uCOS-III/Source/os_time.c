#include "os.h"


void  OSTimeTick (void) //Systick的时钟中断的中断服务函数所调用的就是这个函数，故每触发一次时钟中断执行的就是这个函数
{
	unsigned int i;

	

	/* 扫描就绪列表中所有任务的TaskDelayTicks，如果不为0，则减1 */
	for(i=0; i<OS_CFG_PRIO_MAX; i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)  //如果任务列表中有一个任务的延时时间大于0的话（任务的延时时间的单位是时钟周期，即是还需要延时多少个时钟周期），就使所有的任务的延时时间减1
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks --;
		}
	}
	
	/* 任务调度 */
	OSSched();
}

/* 阻塞延时 */
void  OSTimeDly(OS_TICK dly) //执行这个函数并不会触发时钟中断，仅仅是设置当前任务的延时的时钟周期数，设置好了时钟周期数之后就会调用任务调度函数，根据各个任务的时钟周期数去判断要执行哪一个任务
{
	
	OSTCBCurPtr->TaskDelayTicks = dly; //设置当前正在执行的任务的延时时间
	
	/* 进行任务调度 */
	OSSched();	
}


