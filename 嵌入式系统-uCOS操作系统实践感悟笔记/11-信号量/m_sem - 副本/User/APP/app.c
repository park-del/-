/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>


/*
***************************************************************************
*                             LOCAL DEFINES
***************************************************************************
*/

OS_SEM SemOfKey;          //标志KEY1是否被单击的多值信号量


/*
**************************************************************************
*                                TCB
**************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;                                //任务控制块

static  OS_TCB   AppTaskKeyTCB;
static  OS_TCB   AppTaskLed1TCB;
static  OS_TCB   AppTaskLed2TCB;

/*
***************************************************************************
*                                                STACKS
*************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];       //任务堆栈

static  CPU_STK  AppTaskKeyStk [ APP_TASK_KEY_STK_SIZE ];
static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTaskLed2Stk [ APP_TASK_LED1_STK_SIZE ];

/*
*************************************************************************
*                                         FUNCTION PROTOTYPES
*************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);                       //任务函数声明

static  void  AppTaskKey  ( void * p_arg );
static  void  AppTaskLed1 ( void * p_arg );
static  void  AppTaskLed2 ( void * p_arg );


/*
******************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  
*               It is assumed that your code will callmain() 
*               once you have performed all necessary initialization.
* Arguments   : none
*
* Returns     : none
*****************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                                           //初始化 uC/OS-III

	  /* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    OSStart(&err);                                                          //启动多任务管理（交由uC/OS-III控制）

}


static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
   (void)p_arg;
    BSP_Init();                                                 
    cpu_clk_freq = BSP_CPU_ClkFreq();                           
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        
    OS_CPU_SysTickInit(cnts);                                   
    Mem_Init();                                                                                 
    CPU_IntDisMeasMaxCurReset();                               

    
		/* 创建多值信号量 SemOfKey */
    OSSemCreate((OS_SEM      *)&SemOfKey,    //信号量是一个结构体对象，SemOfKey信号量已经被定义为了全局变量
               (CPU_CHAR    *)"SemOfKey",    //SemOfKey信号量的名字
               (OS_SEM_CTR   )0,             //信号量SemOfKey所指代的初始资源数为0（信号量可以表示一种类型的资源数，也是可以用来对任务间同步进行控制的，这里就是用来表示资源数的） 
               (OS_ERR      *)&err);         
							 

		/* 创建 AppTaskKey 任务（优先级为3） */
    OSTaskCreate((OS_TCB     *)&AppTaskKeyTCB,                              //任务控制块地址
                 (CPU_CHAR   *)"App Task Key",                              //任务名称
                 (OS_TASK_PTR ) AppTaskKey,                                 //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_KEY_PRIO,                          //任务的优先级
                 (CPU_STK    *)&AppTaskKeyStk[0],                           //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_KEY_STK_SIZE / 10,                 //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_KEY_STK_SIZE,                      //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
    
		/* 创建 LED1 任务（优先级为4） */
    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Led1",                             //任务名称
                 (OS_TASK_PTR ) AppTaskLed1,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskLed1Stk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
								 
								 
		/* 创建 LED2 任务（优先级为5）*/
    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                             //任务控制块地址
                 (CPU_CHAR   *)"App Task Led2",                             //任务名称
                 (OS_TASK_PTR ) AppTaskLed2,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) 5,                                          //任务的优先级
                 (CPU_STK    *)&AppTaskLed2Stk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型
								 		 
								 
								 
								 
		/*
		创建了两个任务，其中AppTaskKeyTCB任务是用来“检测按键是否按下”的，AppTaskLed1TCB是用来控制“灯泡亮灭”的。
    AppTaskKeyTCB任务的优先级为APP_TASK_KEY_PRIO（为3），	AppTaskLed1TCB任务的优先级为APP_TASK_LED1_PRIO（为4）。
								 
		创建了这两个任务后这两个任务就会加入到就绪列表中，当执行OSTaskDel()来删除当前正在运行的任务AppTaskStartTCB时，在这个OSTaskDel()函数里面是会进行任务切换的，
		进行任务切换的时候切换到的是“按键检测的任务”。（当自身删除自身的时候肯定是要进行任务切换的，不然就没有任务运行，系统就会出错）
								 
								 
		*/
		
		OSTaskDel ( & AppTaskStartTCB, & err );                     //删除起始任务本身，该任务不再运行，在这里有进行任务切换
}


/*
****************************************************************************
*                                          KEY TASK
****************************************************************************
*/
static  void  AppTaskKey ( void * p_arg )
{
	OS_ERR      err;

	uint8_t ucKey1Press = 0;
	
	
	(void)p_arg;

					 
	while (DEF_TRUE) {                                                         //任务体
		if( Key_Scan ( macKEY1_GPIO_PORT, macKEY1_GPIO_PIN, 1, & ucKey1Press ) ) //如果KEY1被单击
		  OSSemPost((OS_SEM  *)&SemOfKey,                                        //发布SemOfKey
							 (OS_OPT   )OS_OPT_POST_1,                                     //发布给所有等待任务
							 (OS_ERR  *)&err);                                             //返回错误类型	 

		OSTimeDlyHMSM ( 0, 0, 0, 20, OS_OPT_TIME_DLY, & err );                   //每20ms扫描一次（本任务延时的时钟周期数是为20ms的）
	}
	/*
	信号量
	OSSemPost()
	
	*/
	
	
}

static  void  AppTaskLed1 ( void * p_arg )
{
    OS_ERR         err;
	  CPU_INT32U     cpu_clk_freq;
	  CPU_TS         ts_sem_post, ts_sem_get;
    (void)p_arg;
	  cpu_clk_freq = BSP_CPU_ClkFreq();               //获取CPU时钟，时间戳是以该时钟计数
	
	
    while (DEF_TRUE) {                              //任务体
					
			OSSemPend ((OS_SEM   *)&SemOfKey,             //等待该信号量被发布
								 (OS_TICK   )0,                     //无期限等待
								 (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果没有信号量可用就等待
								 (CPU_TS   *)&ts_sem_post,          //获取信号量最后一次被发布的时间戳
								 (OS_ERR   *)&err);                 //返回错误类型
				
			macLED1_TOGGLE ();                            //切换LED1的亮灭状态
    
		}	
}


static  void  AppTaskLed2 ( void * p_arg )
{
    OS_ERR         err;
	  CPU_INT32U     cpu_clk_freq;
	  CPU_TS         ts_sem_post, ts_sem_get;
  
    (void)p_arg;
	  cpu_clk_freq = BSP_CPU_ClkFreq();               //获取CPU时钟，时间戳是以该时钟计数
	
	
    while (DEF_TRUE) {                              //任务体
					
			OSSemPend ((OS_SEM   *)&SemOfKey,             //等待该信号量被发布
								 (OS_TICK   )500,                   //无期限等待
								 (OS_OPT    )OS_OPT_PEND_BLOCKING,  //如果没有信号量可用就等待
								 (CPU_TS   *)&ts_sem_post,          //获取信号量最后一次被发布的时间戳
								 (OS_ERR   *)&err);                 //返回错误类型
				
			macLED2_TOGGLE ();                            //切换LED1的亮灭状态
    }
		
		
}









