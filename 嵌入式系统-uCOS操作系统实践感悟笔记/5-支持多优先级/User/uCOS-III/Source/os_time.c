#include "os.h"


void  OSTimeTick (void)  //这个是时钟中断的函数的，并没有在某一个具体的函数中被调用，而是相对独立的，当触发时钟中断的时候就会去执行这个函数
{
	unsigned int i;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();	//进入临界区
	
	for(i=0; i<OS_CFG_PRIO_MAX; i++) //扫描就绪列表中所有任务的TaskDelayTicks，如果不为0，则减1 	
	{
/*
在就绪列表中是一个元素对应了一个双重链表的，但这里是每一个双重链表中是只有一个结点的，即是每一个优先级对应了一个任务。也是可以进行修改其为一个优先级对应多个任务的，此时
则需要遍历就绪列表中的每一个双重链表，将所有双重链表中TaskDelayTicks不为0的进行减1，然后将所有双重链表中TaskDelayTicks为0的那一个结点（任务）对应的优先级进行减1就行了。
*/		
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks --;
			if(OSRdyList[i].HeadPtr->TaskDelayTicks == 0)
			{
				OS_PrioInsert(i);  //将通过减1之后TaskDelayTicks为0的那一个任务所对应的优先级插入到优先级表中
			}
		}
	}
	
	
	OS_CRITICAL_EXIT();     //退出临界区 

	OSSched();  //切换到就绪列表对应的所有双重链表中优先级最高的那一个结点（任务）的
}


void  OSTimeDly(OS_TICK dly)  //阻塞延时 
{
 
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();  //进入临界区 
	
	OSTCBCurPtr->TaskDelayTicks = dly;  //设置延时时间
	
	OS_PrioRemove(OSTCBCurPtr->Prio); /*调用这个阻塞函数的是为当前任务的，当前任务调用这个阻塞函数则说明了当前任务被阻塞了，此时应该切换到另一个任务去执行，当前所执行的这个任务
	应该是优先级最高的任务，将这个任务优先级对应的优先级表中的相应为置为0后再去执行任务切换函数。 */
	
	OS_CRITICAL_EXIT();  //退出临界区
	
	OSSched();	 //任务调度 
}


