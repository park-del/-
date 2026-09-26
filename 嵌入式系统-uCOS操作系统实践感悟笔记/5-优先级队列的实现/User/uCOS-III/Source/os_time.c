#include "os.h"


void  OSTimeTick (void)
{
	unsigned int i;
	CPU_SR_ALLOC();
	
	
	OS_CRITICAL_ENTER();	//进入临界区 

	/* 扫描就绪列表中所有任务的TaskDelayTicks，如果不为0，则减1 */	
	for(i=0; i<OS_CFG_PRIO_MAX; i++)  /*由于优先级就是为就绪列表的数组下标的，这里是依次对就绪列表的0、1、2、3、……号单元所对应的那个链表中的结点（这里是每个链表只有一个结
	点的，因为每一个优先级都是只对应了一个任务的，OSRdyList[i].HeadPtr就是指就绪列表中指向了优先级为i的那一个任务）进行扫描*/
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks --;
			if(OSRdyList[i].HeadPtr->TaskDelayTicks == 0)	  /*扫描这个就绪列表中的每一个结点，对 TaskDelayTicks > 0，即是延时的任务进行减1，除此之外，还是要判断就绪列表中的每
			一个任务是否被唤醒，即是是否有一个结点的TaskDelayTicks==0，如果有的话就将这个任务所对应的优先级i插入到优先级表中（即是对优先级表中的第i为置1）*/
			{
				OS_PrioInsert(i);
			}
		}
	}
	OS_CRITICAL_EXIT(); 	//退出临界区 
	
	OSSched();	//进行任务调度时所切换的那个任务是为优先级表中最前面的那个1（最高优先级）所对应的就绪列表（i为就绪列表的数组下标）中的那个链表（链表中的每一个结点都是具有相同优先级的一个任务）中的某一个结点的。 
}

/* 阻塞延时 */
void  OSTimeDly(OS_TICK dly)
{

	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();   //进入临界区 
	
	OSTCBCurPtr->TaskDelayTicks = dly;   //设置延时时间（即是延时的时钟周期数的）
	
	OS_PrioRemove(OSTCBCurPtr->Prio);	   //延时了之后就要将当前任务所对应的优先级从优先级表中删除掉（对优先级表中的相应位置0），这样进行调度的时候就不会切换到这个优先级对应的任务了
	
//注意：上面的这步操作会使所有这个优先级的任务都会一起被阻塞，因此这里一般只是适用于一个优先级只对应了一个任务的情况
	
	OS_CRITICAL_EXIT();	//退出临界区 
	
	OSSched();	//任务调度 
}


