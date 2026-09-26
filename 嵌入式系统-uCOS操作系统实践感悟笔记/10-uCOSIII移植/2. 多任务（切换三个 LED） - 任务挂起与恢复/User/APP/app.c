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
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   AppTaskStartTCB;

static  OS_TCB   AppTaskLed1TCB;
static  OS_TCB   AppTaskLed2TCB;
static  OS_TCB   AppTaskLed3TCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  CPU_STK  AppTaskLed1Stk [ APP_TASK_LED1_STK_SIZE ];
static  CPU_STK  AppTaskLed2Stk [ APP_TASK_LED2_STK_SIZE ];
static  CPU_STK  AppTaskLed3Stk [ APP_TASK_LED3_STK_SIZE ];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);

static  void  AppTaskLed1  ( void * p_arg );
static  void  AppTaskLed2  ( void * p_arg );
static  void  AppTaskLed3  ( void * p_arg );
void delay(uint32_t count);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
		
		
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    CPU_IntDisMeasMaxCurReset();


    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                /* Create the Led1 task                                */
                 (CPU_CHAR   *)"App Task Led1",
                 (OS_TASK_PTR ) AppTaskLed1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,
                 (CPU_STK    *)&AppTaskLed1Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                /* Create the Led2 task                                */
                 (CPU_CHAR   *)"App Task Led2",
                 (OS_TASK_PTR ) AppTaskLed2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED2_PRIO,
                 (CPU_STK    *)&AppTaskLed2Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                /* Create the Led3 task                                */
                 (CPU_CHAR   *)"App Task Led3",
                 (OS_TASK_PTR ) AppTaskLed3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
                 (CPU_STK    *)&AppTaskLed3Stk[0],
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
		
		
		OSTaskDel ( & AppTaskStartTCB, & err );
		
		
}


/*
*********************************************************************************************************
*                                          LED1 TASK
*********************************************************************************************************
*/
 
int second=1200000;  //实验测试结果1200000大约就是1s
static  void  AppTaskLed1 ( void * p_arg )
{
      OS_ERR      err;
      int time=second*2;   //time用来控制时间，flag1用来标志AppTaskLed1的状态，flag1为1表示挂起的状态，flag1为0表示现在是苏醒的
      int m=1; 
        
	
     (void)p_arg;


     while (DEF_TRUE) {                                          
			
			if(m==1)           //m1用来作为一个标志位，使得每次都是只有1个任务在运行，其他的都是处于挂起的状态（这个if语句里面的是只执行1次的）
			{
			 OSTaskSuspend(&AppTaskLed2TCB,&err);
			 OSTaskSuspend(&AppTaskLed3TCB,&err);
				
			 m=0;  //防止在执行AppTaskLed1的时候被多次挂起
			}
			
			macLED1_ON ();
		 
			
			time--;            //时间到了之后就会将AppTaskLed1挂起，同时唤醒AppTaskLed2，转去执行AppTaskLed2
			if(time==0)
			{
				macLED1_OFF ();
				time=second*2;
				OSTaskResume(&AppTaskLed2TCB,&err);	  //将AppTaskLed2恢复(注意这里必须是先恢复然后再挂起)		
/*
				这里注意一下，恢复AppTaskLed2TCB的时候也是要进行任务切换的，执行OSTaskResume()任务恢复函数来将AppTaskLed2TCB唤醒时，由于AppTaskLed2TCB的优先级
        与当前正在运行的任务AppTaskLed1TCB的优先级是一样的（AppTaskLed1TCB、AppTaskLed2TCB、AppTaskLed3TCB的优先级都是一样的，且都是为3的），故唤醒
				AppTaskLed2TCB时是将AppTaskLed2TCB插入到就绪列表中数组下标3所对应的链表的尾部的，故此时执行这个OSTaskResume(&AppTaskLed2TCB,&err)唤醒AppTaskLed2TCB
				后执行任务切换函数OSSChed()切换到的还是为AppTaskLed1TCB的。

        然后AppTaskLed1TCB执行下面的OSTaskSuspend()函数将自己挂起（自己处于挂起态），在这个OSTaskSuspend()函数中也是会执行任务切换函数OSSChed()的，此时执行任务
        切换函数OSSChed()切换到的是为	AppTaskLed2TCB，会转去执行AppTaskLed2，此时AppTaskLed1TCB的断点保存在OSTaskSuspend()函数处，当切换回AppTaskLed1TCB的时候
				AppTaskLed1TCB是会从这里接着往下执行的。
*/				
				OSTaskSuspend(&AppTaskLed1TCB,&err);	//将AppTaskLed1挂起
       	
			 
			}			
	
    }
}


/*
*********************************************************************************************************
*                                          LED2 TASK
*********************************************************************************************************
*/


static  void  AppTaskLed2 ( void * p_arg )
{
    OS_ERR      err;
    int time=second*4;  //AppTaskLed2执行的时间为4s，4s后会被挂起，同时唤醒AppTaskLed3

   (void)p_arg;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
			
			macLED2_ON ();	
		  
			time--;
		  if(time==0)  //time时间到了的时候AppTaskLed2就会挂起自己，同时唤醒AppTaskLed3
		  {
				macLED2_OFF ();	
				time=second*4;
			  OSTaskResume(&AppTaskLed3TCB,&err);	  //将AppTaskLed3恢复	
			  OSTaskSuspend(&AppTaskLed2TCB,&err);	//将AppTaskLed2挂起	
			
			
		  }			
			 
    }
}


/*
*********************************************************************************************************
*                                          LED3 TASK
*********************************************************************************************************
*/

static  void  AppTaskLed3 ( void * p_arg )
{
    OS_ERR      err;
    int time=second*8;

   (void)p_arg;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
		
		macLED3_ON ();
			
	  time--;
		if(time==0)
		{
			macLED3_OFF ();
			time=second*8;
			OSTaskResume(&AppTaskLed1TCB,&err);	  //将AppTaskLed1恢复	
			OSTaskSuspend(&AppTaskLed3TCB,&err);	//将任务3挂起		
		}			
 
		
  }
		
		
}


