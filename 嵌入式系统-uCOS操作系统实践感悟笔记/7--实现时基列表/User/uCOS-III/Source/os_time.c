#include "os.h"


void  OSTimeTick (void) //每触发一次时钟中断就会执行一次这个函数
{
	OS_TickListUpdate();	//更新时基列表,此时可能唤醒某些任务，即是将任务从时基列表中删除，同时将这个任务加入到就绪列表中（在就绪列表的插入操作中是有对优先级表中的相应位置1的操作的）
	OSSched();	          //进行任务调度 
}


void  OSTimeDly(OS_TICK dly)  //阻塞延时
{
	CPU_SR_ALLOC();
	
 
	//当前任务调用了这个函数说明了当前任务被阻塞了
	OS_CRITICAL_ENTER(); //进入临界区
 
	
	OS_TickListInsert(OSTCBCurPtr, dly);//将当前任务OSTCBCurPtr插入到时基列表 
	

	OS_RdyListRemove(OSTCBCurPtr);	    //从就绪列表移除当前任务OSTCBCurPtr（创建任务的时候就会将这个任务加入到就绪列表中了），此时是有对当前任务所对应的优先级表中相应的位置为0的
	

	OS_CRITICAL_EXIT();	//退出临界区
	
  OSSched();		//进行任务调度时还是按照优先级来进行任务调度的，此时由于当前任务(被阻塞)在优先级表中相应的位被置为了0，故此时进行任务调度的时候是不会再调用当前任务的 
	
}


