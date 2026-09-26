/*
************************************************************************************************************************
*                                                 包含的头文件
************************************************************************************************************************
*/
#include "os.h"
#include "ARMCM3.h"

/*
************************************************************************************************************************
*                                                   宏定义
************************************************************************************************************************
*/


/*
************************************************************************************************************************
*                                                  全局变量
************************************************************************************************************************
*/

uint32_t flag1;
uint32_t flag2;
uint32_t flag3;
uint32_t flag4;
/*
************************************************************************************************************************
*                                                  TCB & STACK & 任务声明
************************************************************************************************************************
*/
#define  TASK1_STK_SIZE       128
#define  TASK2_STK_SIZE       128
#define  TASK3_STK_SIZE       128
#define  TASK4_STK_SIZE       128


static   OS_TCB    Task1TCB;
static   OS_TCB    Task2TCB;
static   OS_TCB    Task3TCB;
static   OS_TCB    Task4TCB;

static   CPU_STK   Task1Stk[TASK1_STK_SIZE];
static   CPU_STK   Task2Stk[TASK2_STK_SIZE];
static   CPU_STK   Task3Stk[TASK3_STK_SIZE];
static   CPU_STK   Task4Stk[TASK4_STK_SIZE];

void     Task1( void *p_arg );
void     Task2( void *p_arg );
void     Task3( void *p_arg );
void     Task4( void *p_arg);

/*
************************************************************************************************************************
*                                                  函数声明
************************************************************************************************************************
*/
void delay(uint32_t count);

/*
************************************************************************************************************************
*                                                    main函数
************************************************************************************************************************
*/
/*
* 注意事项：1、该工程使用软件仿真，debug需选择 Ude Simulator
*           2、在Target选项卡里面把晶振Xtal(Mhz)的值改为25，默认是12，
*              改成25是为了跟system_ARMCM3.c中定义的__SYSTEM_CLOCK相同，确保仿真的时候时钟一致
*/
int main(void)
{	
	OS_ERR err;	
	
	
	/* CPU初始化：1、初始化时间戳 */
	CPU_Init();	
	
	/* 关闭中断 */
	CPU_IntDis();
	
	/* 配置SysTick 10ms 中断一次 */
	OS_CPU_SysTickInit (10);
	
	/* 初始化相关的全局变量 */
	OSInit(&err);
	
	/* 创建任务 */
	OSTaskCreate( (OS_TCB       *)&Task1TCB, 
	              (OS_TASK_PTR   )Task1, 
                  (void         *)0,
				        (OS_PRIO       )1,  //优先级为1
				        (CPU_STK      *)&Task1Stk[0],
				        (CPU_STK_SIZE  )TASK1_STK_SIZE,
				        (OS_TICK       )0,  //时间片为0个时钟周期
				        (OS_ERR       *)&err );
				  
	OSTaskCreate( (OS_TCB       *)&Task2TCB, 
	              (OS_TASK_PTR   )Task2, 
                  (void         *)0,
				  (OS_PRIO       )3,      //优先级为3
				  (CPU_STK      *)&Task2Stk[0],
				  (CPU_STK_SIZE  )TASK2_STK_SIZE,
				  (OS_TICK       )1,      //时间片为1个时钟周期
				  (OS_ERR       *)&err );	
				  
	OSTaskCreate( (OS_TCB       *)&Task3TCB, 
	              (OS_TASK_PTR   )Task3, 
                  (void         *)0,
				  (OS_PRIO       )3,      //优先级为3
				  (CPU_STK      *)&Task3Stk[0],
				  (CPU_STK_SIZE  )TASK3_STK_SIZE,
				  (OS_TICK       )1,      //时间片为1个时钟周期
				  (OS_ERR       *)&err ); 
										OSTaskCreate( (OS_TCB       *)&Task4TCB, 
	
	(OS_TASK_PTR   )Task4, 
                  (void         *)0,
				  (OS_PRIO       )2,      //优先级为2
				  (CPU_STK      *)&Task4Stk[0],
				  (CPU_STK_SIZE  )TASK4_STK_SIZE,
				  (OS_TICK       )0,      //时间片为0个时钟周期
				  (OS_ERR       *)&err ); 
				  
	/* 启动OS，将不再返回 */				
	OSStart(&err);
}

/*
************************************************************************************************************************
*                                                    函数实现
************************************************************************************************************************
*/
/* 软件延时 */
void delay (uint32_t count)
{
	for(; count!=0; count--);
}
void Task1( void *p_arg )
{	OS_ERR err;
	  for( ;; ){
		flag1 = 1;
		OSTimeDly(2);	
    
		OSTaskSuspend(&Task3TCB,&err);	//将任务3挂起		
		
		flag1 = 0;
		OSTimeDly(2);	
    
		OSTaskResume(&Task3TCB,&err);	  //将任务3恢复
	}
}

void Task2( void *p_arg )
{
	for( ;; )
    {
		flag2 = 1;
		delay(0xff);		
		flag2 = 0;
		delay(0xff);
	}
}

void Task3( void *p_arg )
{
	for( ;; )
    {
		flag3 = 1;
		delay(0xff);		
		flag3 = 0;
		delay(0xff);
	}
}
void Task4( void *p_arg )
{	  
	 OS_ERR err;
	  for( ;; ){
		flag4 = 1;
	  OSTimeDly(2);	
    
		OSTaskSuspend(&Task2TCB,&err);	//将任务3挂起		
		
		flag4 = 0;
	  OSTimeDly(2);	
			
		OSTaskResume(&Task2TCB,&err);	  //将任务3恢复
	}
}
/*
第1个时钟周期
Task1执行OSTimeDly(2)之前：
就绪位表：0x7（0111） 000000 1（0001）                                         数组下标1：Task1         数组下标2：Task4                数组下标3：Task2   Task3

Task1执行OSTimeDly(2)之后（Task1延时）：
就绪位表：0x3（0011） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                数组下标3：Task2   Task3
转去执行Task4

Task4执行OSTimeDly(2)之前：
就绪位表：0x3（0011） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                数组下标3：Task2   Task3


Task4执行OSTimeDly(2)之后（Task4延时）：
就绪位表：0x1（0001） 000000 1（0001）                                         数组下标1：没有          数组下标2：没有                 数组下标3：Task2   Task3
转去执行Task2

-1-分析第1个时钟周期发生的事：Task1优先级最高，先执行，令flag1由0变为1，执行了OSTimeDly(2)阻塞，在这个OSTimeDly(2)函数中进行了任务切换；切换到Task4执行（Task4的优先级比
Task2和Task3高），令flag4由0变为1，执行了OSTimeDly(2)阻塞，在这个OSTimeDly(2)函数中进行了任务切换；切换到Task2去执行，Task2执行1个时钟周期（Task2的时间片为1个时钟周期）
后触发了时钟中断。


第2个时钟周期（触发第1次时钟中断）
就绪位表：0x1（0001） 000000 1（0001）                                         数组下标1：没有          数组下标2：没有                 数组下标3：Task3   Task2

-2-分析第2个时钟周期发生的事：触发了时钟中断后，在这个时钟中断中对Task1和Task4的延时时钟周期数减1，发现Task1和Task4还是处于延时的，进行任务切换时切换到的是Task3的。

第3个时钟周期（触发第2次时钟中断），Task1苏醒，Task4苏醒
Task1执行OSTaskSuspend(&Task3TCB,&err)之后，执行OSTimeDly(2)之前：
就绪位表：0x7（0111） 000000 1（0001）                                         数组下标1：Task1         数组下标2：Task4                数组下标3：Task2

Task1执行OSTimeDly(2)之后（Task1延时）：
就绪位表：0x3（0011） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                 数组下标3：Task2   
转去执行Task4

Task4执行OSTaskSuspend(&Task2TCB,&err)之后，执行OSTimeDly(2)之前：
就绪位表：0x2（0010） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                 数组下标3：没有                                    

Task4执行OSTimeDly(2)之后（Task4延时）：
就绪位表：0x0（0000） 000000 1（0001）                                         数组下标1：没有          数组下标2：没有                  数组下标3：没有     （补充）数组下标31：空闲Task                                

-3-分析第3个时钟周期发生的事：触发了时钟中断后（这是第2次时钟中断了），在时钟中断中唤醒了Task1，Task4，在时钟中断中进行了任务切换，切换到了Task1。Task1从原来的OSTimeDly(2)
这个断点处接着往下执行，执行OSTaskSuspend(&Task3TCB,&err)将Task3挂起（从就绪列表中移除），令flag1由原来的1变为0，执行了OSTimeDly(2)阻塞，在这个OSTimeDly(2)函数中进行了任
务切换；切换到了Task4，Task4从原来的断点处接着执行，执行OSTaskSuspend(&Task2TCB,&err)将Task2挂起（从就绪列表中移除），令flag4由原来的1变为0，执行了OSTimeDly(2)阻塞，在这个
OSTimeDly(2)函数中进行了任务切换；切换到空闲任务去执行（此时就绪列表中就只有空闲任务的）。


第4个时钟周期（触发第3次时钟中断）
就绪位表：0x0（0000） 000000 1（0001）                                         数组下标1：没有          数组下标2：没有                 数组下标3：没有      （补充）数组下标31：Task空闲 

-4-分析第4个时钟周期发生的事：触发了时钟中断（这里是第3次时钟中断）后，进入到第4个时钟周期，在时钟中断中进行了任务切换，此时Task1和Task4阻塞，Task2和Task3被从就绪列表中移除
了，此时就绪列表中只有空闲任务，故此时在时钟中断进行任务切换时切换到的还是空闲任务的。

第5个时钟周期（触发第4次时钟中断），Task1苏醒，Task4苏醒
Task1执行OSTaskResume(&Task3TCB,&err)之后，执行OSTimeDly(2)之前：
就绪位表：0x7（0111） 000000 1（0001）                                         数组下标1：Task1          数组下标2：Task4                数组下标3：Task3

Task1执行OSTimeDly(2)之后（Task1延时）：
就绪位表：0x3（0011） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                  数组下标3：Task3
转去执行Task4

Task4执行OSTaskResume(&Task2TCB,&err)之后，执行OSTimeDly(2)之前：
就绪位表：0x3（0011） 000000 1（0001）                                         数组下标1：没有          数组下标2：Task4                数组下标3：Task2   Task3

Task4执行OSTimeDly(2)之后（Task4延时）：
就绪位表：0x1（0001） 000000 1（0001）                                         数组下标1：没有          数组下标2：没有                  数组下标3：Task2   Task3

-5-分析第5个时钟周期发生的事：触发了时钟中断后（这里是第4次时钟中断）后，在这个时钟中断中唤醒了Task1和Task4，进行了任务切换。切换到了Task1去执行,Task1执行了
OSTaskResume(&Task3TCB,&err)函数恢复了Task3（将Task3重新插入到就绪列表中），令flag1由0变为1，执行了OSTimeDly(2)函数，在这个OSTimeDly(2)函数中进行了任务切换；
切换到Task4去执行，Task4执行了OSTaskResume(&Task2TCB,&err)函数恢复了Task2（将Task2重新插入到就绪列表中，注意，由于Task2的优先级与当前正在运行的Task4的优先级不一样
，故Task2是插入到对应的链表的头部的，即Task2是插入到Task3的头部的），令flag4由0变为1，执行了OSTimeDly(2)函数，在这个OSTimeDly(2)函数中进行了任务切换；切换到了Task2
去执行。

*/







 