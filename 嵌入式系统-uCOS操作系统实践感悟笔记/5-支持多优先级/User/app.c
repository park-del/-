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

/*
************************************************************************************************************************
*                                                  TCB & STACK & 任务声明
************************************************************************************************************************
*/
#define  TASK1_STK_SIZE       128
#define  TASK2_STK_SIZE       128
#define  TASK3_STK_SIZE       128


static   OS_TCB    Task1TCB;
static   OS_TCB    Task2TCB;
static   OS_TCB    Task3TCB;


static   CPU_STK   Task1Stk[TASK1_STK_SIZE];
static   CPU_STK   Task2Stk[TASK2_STK_SIZE];
static   CPU_STK   Task3Stk[TASK2_STK_SIZE];


void     Task1( void *p_arg );
void     Task2( void *p_arg );
void     Task3( void *p_arg );


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

int main(void)
{	
	OS_ERR err;	
	CPU_Init();	
	CPU_IntDis();
	OS_CPU_SysTickInit (10);
	OSInit(&err);
	
	OSTaskCreate( (OS_TCB*)&Task1TCB, 
	              (OS_TASK_PTR )Task1, 
                  (void *)0,
				  (OS_PRIO)1,
				  (CPU_STK*)&Task1Stk[0],
				  (CPU_STK_SIZE)  TASK1_STK_SIZE,
				  (OS_ERR *)&err );
				  
	OSTaskCreate( (OS_TCB*)&Task2TCB, 
	              (OS_TASK_PTR )Task2, 
                  (void *)0,
				  (OS_PRIO)2,
				  (CPU_STK*)&Task2Stk[0],
				  (CPU_STK_SIZE)  TASK2_STK_SIZE,
				  (OS_ERR *)&err );	
				  
 							
	OSTaskCreate( (OS_TCB*)&Task3TCB, 
	              (OS_TASK_PTR )Task3, 
                  (void *)0,
				  (OS_PRIO)3,
				  (CPU_STK*)&Task3Stk[0],
				  (CPU_STK_SIZE)  TASK3_STK_SIZE,
				  (OS_ERR *)&err );
/*
说明：在创建一个任务的时候就是会将这个任务对应的优先级在优先级表中置位的，故在创建了上述3个任务之后此时的优先级表应该是为
      0x 7（0111） 000000 1（0001）的。
		
注意：空闲任务的创建是在操作系统初始化的时候完成的，即是在执行OSInit(&err)这个函数的时候完成了对空闲任务的初始化，在执行这个函数之后优先级表中的最后一位就被置为了1的。									
*/									
					
	OSStart(&err); //启动操作系统的时候切换到的是最高优先级的那个任务的，即是启动操作系统时会执行Task1
}

 
void delay (uint32_t count) //软件延时
{
	for(; count!=0; count--);
}

/*
分析:
<1>Task1的优先级为1，Task2的优先级为2，Task3的优先级为3，启动操作系统后执行的是为Task1的。


<2>Task1执行完OSTimeDly(1)时，会设置Task1的时钟周期数为1，然后在优先级表中移除Task1的优先级，再进行任务切换，此时切换到的应该是为Task2的。

变量的值：此时优先级表OSPrio为0x 3（0011） 000000 1（0001），当前任务的优先级OSPrioCur为0x02，优先级表中的最高优先级OSPrioHighRdy为0x02（最高优先级就是为当前优先级的）
         指向当前任务的指针OSTCBCurPtr为指向Task2TCB，指向要切换到的任务的指针OSTCBHighRdyPtr也是指向Task2TCB的。

<3>Task1执行完OSTimeDly(1)后进入阻塞，切换到Task2执行，Task2执行完OSTimeDly(1)后也是会进入阻塞的，此时切换到的应该是为Task3的。

变量的值：此时优先级表OSPrio为0x 1（0001） 000000 1（0001），当前任务的优先级OSPrioCur为0x03，优先级表中的最高优先级OSPrioHighRdy为0x03（最高优先级就是为当前优先级的）
         指向当前任务的指针OSTCBCurPtr为指向Task3TCB，指向要切换到的任务的指针OSTCBHighRdyPtr也是指向Task3TCB的。

<4>Task2执行完OSTimeDly(1)后进入阻塞，切换到Task3执行，Task3执行完OSTimeDly(1)后也是会进入阻塞的，此时切换到的应该是为OSIdleTaskTCB的,即此时所切换的应该是为空闲任务的。

变量的值：此时优先级表OSPrio为0x 0（0000） 000000 1（0001），当前任务的优先级OSPrioCur为0x1F（十进制为31），优先级表中的最高优先级OSPrioHighRdy为0x1F（最高优先级就是为
当前优先级的）指向当前任务的指针OSTCBCurPtr为指向OSIdleTaskTCB，指向要切换到的任务的指针OSTCBHighRdyPtr也是指向OSIdleTaskTCB的。

<5>此时Task1、Task2、Task3都是被阻塞，执行的是空闲任务，空闲任务一直进行循环，等到触发一次时钟中断的时候去执行中断服务函数OSTimeTick，将就绪列表中所有双重链表的结点（一个
   结点对应了一个任务）的TaskDelayTicks > 0的进行减1，然后将TaskDelayTicks为0的所有结点对应的优先级在优先级表中置位1，然后进行任务切换，切换到优先级表中最高优先级的那个任务。

变量的值：此时优先级表OSPrio为0x 3（0011） 000000 1（0001），当前任务的优先级OSPrioCur为0x01，优先级表中的最高优先级OSPrioHighRdy为0x01（最高优先级就是为当前优先级的）
         指向当前任务的指针OSTCBCurPtr为指向Task1TCB，指向要切换到的任务的指针OSTCBHighRdyPtr也是指向Task1TCB的。*/

void Task1( void *p_arg )
{	
	for( ;; )
    {
		flag1 = 1;
		
//执行flag1=1之后，OSPrioCur为 0x 7（0111）000000 1（0001），OSPrioCur为0x01，OSPrioHighRdy为0x01，OSTCBCurPtr指向Task1TCB，
//OSTCBHighRdyPtr指向Task1TCB。			
			
		OSTimeDly(1);		 //移除了优先级表中第2位，进行任务切换到任务2
		flag1 = 0;
		OSTimeDly(1);		
	}
}

void Task2( void *p_arg )
{
	for( ;; )
    {
		flag2 = 1;  
	
//执行flag2=1之后（说明已经执行过了flag1=1和OSTimeDly(1)），OSPrioCur为 0x 3（0011）000000 1（0001），OSPrioCur为0x02，OSPrioHighRdy为0x02，OSTCBCurPtr指向Task2TCB，
//OSTCBHighRdyPtr指向Task2TCB。
			
		OSTimeDly(1);		//移除了优先级表中第3位，进行任务切换到任务3
		flag2 = 0;			
			
//执行flag2=0之后（说明已经执行过了1次时钟中断的中断服务函数，已经执行过flag1=0、OSTimeDly(1)了），OSPrioCur为 0x 3（0011）000000 1（0001），OSPrioCur为0x02，OSPrioHighRdy
//为0x02，OSTCBCurPtr指向Task2TCB，OSTCBHighRdyPtr指向Task2TCB。	
			
		OSTimeDly(1);
	}
}

void Task3( void *p_arg )
{
	for( ;; )
    {
			
//执行flag3=1之前（说明已经执行过了flag1=1、OSTimeDly(1)和flag2=1、OSTimeDly(1）），OSPrioCur为 0x 1（0001）000000 1（0001），OSPrioCur为0x03，OSPrioHighRdy为0x03，
//OSTCBCurPtr指向Task3TCB，OSTCBHighRdyPtr指向Task3TCB。		
			
		flag3 = 1;   		
		
		OSTimeDly(1);  //移除了优先级表中第4位，进行任务切换到空闲任务		
			
//OSTimeDly(1)之后（说明已经执行过了flag1=0、OSTimeDly(1),flag2=0、OSTimeDly(1)）,此时OSPrioCur为 0x 1（0001）000000 1（0001），OSPrioCur为0x03，OSPrioHighRdy为0x03，
//OSTCBCurPtr指向Task3TCB，OSTCBHighRdyPtr指向Task3TCB。		
			
		flag3 = 0;
		OSTimeDly(1);
	}
}

 

