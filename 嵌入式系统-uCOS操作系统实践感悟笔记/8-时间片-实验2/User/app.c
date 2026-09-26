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
void     Task4( void *p_arg );


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
	
	CPU_Init();	/* CPU初始化：1、初始化时间戳 */
	
	CPU_IntDis();	/* 关闭中断 */
	
	OS_CPU_SysTickInit (10);	/* 配置SysTick 10ms 中断一次 */
	
	OSInit(&err);/* 初始化相关的全局变量 */
	
	/* 创建任务 */
	OSTaskCreate( (OS_TCB       *)&Task1TCB, 
	              (OS_TASK_PTR   )Task1, 
                  (void         *)0,
				        (OS_PRIO       )1,
				        (CPU_STK      *)&Task1Stk[0],
				        (CPU_STK_SIZE  )TASK1_STK_SIZE,
				        (OS_TICK       )0,
				        (OS_ERR       *)&err );
				  
	OSTaskCreate( (OS_TCB       *)&Task2TCB, 
	              (OS_TASK_PTR   )Task2, 
                  (void         *)0,
				  (OS_PRIO       )2,
				  (CPU_STK      *)&Task2Stk[0],
				  (CPU_STK_SIZE  )TASK2_STK_SIZE,
				  (OS_TICK       )1,
				  (OS_ERR       *)&err );	
				  
	OSTaskCreate( (OS_TCB       *)&Task3TCB, 
	              (OS_TASK_PTR   )Task3, 
                  (void         *)0,
				  (OS_PRIO       )2,
				  (CPU_STK      *)&Task3Stk[0],
				  (CPU_STK_SIZE  )TASK3_STK_SIZE,
				  (OS_TICK       )1,
				  (OS_ERR       *)&err ); 
									
	OSTaskCreate( (OS_TCB       *)&Task4TCB, 
	              (OS_TASK_PTR   )Task4, 
                  (void         *)0,
				  (OS_PRIO       )2,
				  (CPU_STK      *)&Task4Stk[0],
				  (CPU_STK_SIZE  )TASK4_STK_SIZE,
				  (OS_TICK       )2,
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
{	
	for( ;; )
    {
		flag1 = 1;
		OSTimeDly(2);		
		flag1 = 0;
		OSTimeDly(2);		
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

void Task4( void *p_arg)
{
  for( ;; )
  {
		flag4 = 1;
		OSTimeDly(1);
		flag4 = 0;
		OSTimeDly(1);
	}
}
/*

 

 
(1)刚开始时：第1个时钟周期时  
执行OSTimeDly(2)之前：
运行：Task1
就绪位表：0x7（0111） 000000 1（0001）  就绪列表：数组下标为1：Task1   数组下标为2：Task2 Task3  数组下标3：Task4

执行OSTimeDly(2)之后：
运行：Task2
就绪位表：0x3（0011） 000000 1（0001）  就绪列表：数组下标为1：没有    数组下标为2：Task2 Task3  数组下标3：Task4

(2)第1次时钟中断触发后：第2个时钟周期时
运行：Task3
就绪位表：0x3（0011） 000000 1（0001）  就绪列表：数组下标为1：没有    数组下标为2：Task3 Task2  数组下标3：Task:4

(3)第2次时钟中断触发后：第3个时钟周期时
执行OSTimeDly(2)之前：
运行：Task1
就绪位表：0x7（0111） 000000 1（0001）  就绪列表：数组下标为1：Task1   数组下标为2：Task4 Task2  数组下标3：Task4

执行OSTimeDly(2)之后 
运行：Task4
就绪位表：0x3（0011） 000000 1（0001）  就绪列表：数组下标为1：没有    数组下标为2：Task4 Task2  数组下标3：Task4




*/


