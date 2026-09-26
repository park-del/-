//发送信号量SemOfKey的函数，当按键检测到按键按下时就会执行这个函数发送SemOfKey信号，发送之前首先是要调用OSSemCreate()函数来创建SemOfKey信号量。
OS_SEM_CTR  OSSemPost (OS_SEM  *p_sem,    //信号量SemOfKey
                       OS_OPT   opt,      //选项
                       OS_ERR  *p_err)    //返回错误类型
{
    OS_SEM_CTR  ctr;
    CPU_TS      ts;

    ts = OS_TS_GET();                             //获取时间戳（调用这个函数可以获取信号量SemOfKey的时间戳，SemOfKey的时间戳就是SemOfKey最后释放的时间的，即是最后一次执行OSSemPost()函数时的的时钟周期数）

#if OS_CFG_ISR_POST_DEFERRED_EN > 0u              //如果使能了中断延迟发布
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {    //如果该函数是在中断中被调用
        OS_IntQPost((OS_OBJ_TYPE)OS_OBJ_TYPE_SEM, //将该信号量发布到中断消息队列
                    (void      *)p_sem,
                    (void      *)0,
                    (OS_MSG_SIZE)0,
                    (OS_FLAGS   )0,
                    (OS_OPT     )opt,
                    (CPU_TS     )ts,
                    (OS_ERR    *)p_err);
        return ((OS_SEM_CTR)0);                   //返回0（尚未发布），不继续执行        
    }
#endif

    ctr = OS_SemPost(p_sem,                       //将信号量按照普通方式处理（如果 OSSemPost()这个函数没有在中断中被调用的话就会执行这个函数）
                     opt,
                     ts,
                     p_err);

    return (ctr);                                 //返回信号的当前计数值
}



/*发送SemOfKey信号量（发送信号量就是释放信号量，信号量SemOfKey的资源数要加1）的时候肯定是要判断是否有任务在等待当前这个信号量的，
  如果有的话则是要从这个信号量的等待列表中唤醒一个任务的。*/
OS_SEM_CTR  OS_SemPost (OS_SEM  *p_sem, //SemOfKey信号量指针，p_sem指向了SemOfKey信号量
                        OS_OPT   opt,   //选项
                        CPU_TS   ts,    //时间戳
                        OS_ERR  *p_err) //返回错误类型
{
    OS_OBJ_QTY     cnt;
    OS_SEM_CTR     ctr;    //从这里就可以看到OS_SEM_CTR是为ctr变量的类型的，则sizeof(OS_SEM_CTR)就是为这个类型的字节数的
    OS_PEND_LIST  *p_pend_list;
    OS_PEND_DATA  *p_pend_data;
    OS_PEND_DATA  *p_pend_data_next;
    OS_TCB        *p_tcb;
    CPU_SR_ALLOC();



    CPU_CRITICAL_ENTER();                                   //关中断
    p_pend_list = &p_sem->PendList;                         //取出SemOfKey信号量的等待列表
	
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) {         //如果没有任务在等待该SemOfKey信号量
			
			  //这个switch语句就是用来判断是否发生溢出的，如果没有发生溢出的话就不用管
        switch (sizeof(OS_SEM_CTR)) {                      
            case 1u:                                        
                 if (p_sem->Ctr == DEF_INT_08U_MAX_VAL) {   /*“计数值溢出”，返回0（有错误）， 即是OS_SEM_CTR如果为1u的话
									 （OS_SEM_CTR表示信号量的Ctr变量的类型），就是为1个无符号数，此时如果p_sem->Ctr值为255的话，则其再进行加1的话就会发生溢出。*/
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            case 2u:
                 if (p_sem->Ctr == DEF_INT_16U_MAX_VAL) {
                     CPU_CRITICAL_EXIT();
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            case 4u:
                 if (p_sem->Ctr == DEF_INT_32U_MAX_VAL) {
                     CPU_CRITICAL_EXIT();
                    *p_err = OS_ERR_SEM_OVF;
                     return ((OS_SEM_CTR)0);
                 }
                 break;

            default:
                 break;
        }
				
        p_sem->Ctr++;                                       //信号量计数值不溢出则加1
				
        ctr       = p_sem->Ctr;                             //获取信号量计数值到 ctr
				
        p_sem->TS = ts;                                     //保存时间戳（时间戳表示记录本次所释放信号量的时间）
				
        CPU_CRITICAL_EXIT();                                //开中断
       *p_err     = OS_ERR_NONE;                            //返回错误类型为“无错误”
        return (ctr);                                       //返回信号量的计数值，不继续执行（此时直接返回就行了，因为SemOfKey信号量的等待队列中没有任务需要被唤醒）
    }

		//执行下面的说明了SemOfKey信号量的等待队列中有正在等待SemOfKey信号量的任务
    OS_CRITICAL_ENTER_CPU_EXIT();                           //加锁调度器，但开中断
		
    if ((opt & OS_OPT_POST_ALL) != (OS_OPT)0) {             /*如果要将SemOfKey发布给所有等待任务（由于是可以选择 释放信号量 时不进行任务切换的，即在执行本OS_SemPost()函数
		中有一个opt选项，可以通过这个选项来指明是唤醒等待队列中的一个任务，还是多个任务。
		故在执行OS_SemPost()函数SemOfKey中的资源数是可能有多个的，故是可以是可以同时唤醒多个任务的，这里选择的是唤醒多个任务）*/
        cnt = p_pend_list->NbrEntries;                      //获取等待任务数目到 cnt
    } 
		else {                                                  //如果要将信号量发布给优先级最高的等待任务（只唤醒一个等待的任务）
        cnt = (OS_OBJ_QTY)1;                                //将要操作的任务数为1，cnt 置1
    }
		
    p_pend_data = p_pend_list->HeadPtr;                     //获取SemOfKey信号量等待列表的首个任务p_pend_data（即是等待队列中的第1个结点）
		
    while (cnt > 0u) {                                      //逐个处理要唤醒任务（在插入信号量的等待队列时是按照由优先级的顺序从前往后排的），cnt就是资源数，也就是要唤醒的任务数
        p_tcb            = p_pend_data->TCBPtr;             //取出要唤醒的任务p_tcb
        p_pend_data_next = p_pend_data->NextPtr;            //取出下一个任务
        
			OS_Post((OS_PEND_OBJ *)((void *)p_sem),               /*发布信号量SemOfKey给p_tcb，即是唤醒当前任务（在这个函数里面进行的只是将相应的任务从一个表中删除，
			插入到另外的一个表，是并没有进行任务切换的）*/
                p_tcb,
                (void      *)0,
                (OS_MSG_SIZE)0,
                ts);
        p_pend_data = p_pend_data_next;                     //处理下一个任务          
        cnt--;                                              //cnt是单独定义的一个变量用来记录SemOfKey信号量中的资源数的，此时这个资源数是要进行减1的，因为已经唤醒了一个任务
    }
		
		//上面其实就是用来指明执行这个函数每次唤醒SemOfKey信号量的等待队列中的一个任务还是多个任务
    ctr = p_sem->Ctr;                                       //获取信号量计数值到 ctr
    OS_CRITICAL_EXIT_NO_SCHED();                            //减锁调度器，但不执行任务调度
    
		if ((opt & OS_OPT_POST_NO_SCHED) == (OS_OPT)0) {        //进行任务调度
        OSSched();                                          
    }
   *p_err = OS_ERR_NONE;                                    //返回错误类型为“无错误”
    return (ctr);                                           //返回信号量的当前计数值
}
