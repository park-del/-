 OS_SEM_CTR  OSSemPend (OS_SEM   *p_sem,   //信号量SemOfKey
                       OS_TICK   timeout, //等待超时时间
                       OS_OPT    opt,     //选项
                       CPU_TS   *p_ts,    //等到信号量时的时间戳
                       OS_ERR   *p_err)    
{
    OS_SEM_CTR    ctr;
    OS_PEND_DATA  pend_data;
    CPU_SR_ALLOC();

		
    CPU_CRITICAL_ENTER();                     //关中断
   
    if (p_sem->Ctr > (OS_SEM_CTR)0) {         //如果SemOfKey中的资源数不为0的话，则当前任务肯定能申请到资源，是不会阻塞的，直接返回就可以了
        
			  p_sem->Ctr--;                         //SemOfKey中的资源数减1
			
        if (p_ts != (CPU_TS *)0) {            //如果 p_ts 非空
           *p_ts  = p_sem->TS;                //获取SemOfKey最后一次释放的时间戳，用*p_ts记录，
        }
				
        ctr   = p_sem->Ctr;                   //获取SemOfKey的当前资源数目
        
				CPU_CRITICAL_EXIT();                  //开中断
       *p_err = OS_ERR_NONE;                  //返回错误类型为“无错误”
        return (ctr);                         //返回信号量SemOfKey中的的当前资源数目，不继续执行，因为当前任务已经获取到了这个SemOfKey中的资源了
    }

		/*不满足上面那个“if条件”说明了 p_sem->Ctr为<=0 ，即信号量SemOfKey没有资源（此时这个获取信号量SemOfKey中的资源的任务根据opt选项有两种选择，一种是申请不到资源的时候就直接
		  返回，不进行阻塞，继续接着原来的运行；另一种是申请不到资源时就进行阻塞，从就绪列表移除到SemOfKey对应的等待列表，一直进行等待，同时也会将其加入到一个时基列表中的，设置一个
		   等待的时间timeout，当等待的时间到达还没有处于就绪列表，即是还没有获取到信号量时就会自动的苏醒转为就绪态，加入到就绪列表）*/
	
		
    if ((opt & OS_OPT_PEND_NON_BLOCKING) != (OS_OPT)0) {    //当前任务选择了不阻塞
       
			ctr   = p_sem->Ctr;                                 //获取信号量的资源数目，用ctr来进行记录
        
			CPU_CRITICAL_EXIT();                                  //开中断
       *p_err = OS_ERR_PEND_WOULD_BLOCK;                    //返回错误类型为“等待渴求堵塞”  
        return (ctr);                                       //返回SemOfKey的当前资源数目，不继续执行，该任务继续转去执行原来的
    } 
		else //选择了阻塞任务
		{                                                       
       
  			if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {    /*如果调度器被上锁，调度器被上了锁的话是不能够进行执行OSSched()函数进行任务切换的，如果此时调度器上锁了又选择了没
				有申请到资源时阻塞该任务，则此时就会出现没有任务执行的情况，系统就会出现问题。此时这中情况只能够是执行return返回到主程序。同时返回一个错误“调度器被锁”，告诉原来的主程序
				你虽然选择了opt为“没有申请到时阻塞，一直等待”，但是我此时没有办法去执行这个操作，因为调度器被上锁了。*/
            
					 CPU_CRITICAL_EXIT();                             //开中断
           *p_err = OS_ERR_SCHED_LOCKED;                    //返回错误类型为“调度器被锁”
            return ((OS_SEM_CTR)0);                         //返回0（有错误），不继续执行
        }
    }
    
    /*上面都不满足说明了没有资源可用，选择了阻塞任务，调度器并没有被上锁，是可以进行任务切换的，此时会将这个没有申请到资源的当前任务进行阻塞，进行阻塞时会将当前这个任务从就绪列表
		  中移除到信号量SemOfKey的等待列表中，同时将这个任务加入到时基列表中，设置其睡眠的时间，也即是等待时间为timeout，当timeout时间到时还是在等待SemOfKey信号量时，此时在时钟中断
		  就会将这个任务给唤醒。*/
	 
    OS_CRITICAL_ENTER_CPU_EXIT();                           //锁调度器 ，不能进行任务调度
    
		OS_Pend(&pend_data,                                     //阻塞当前任务，将当前任务脱离就绪列表，
            (OS_PEND_OBJ *)((void *)p_sem),                 //并插入到时基列表和等待列表，同时设置其等待的时间为timeout。
            OS_TASK_PEND_ON_SEM,
            timeout);

 					
    OS_CRITICAL_EXIT_NO_SCHED();                            //开调度器，能进行任务调度

    OSSched();                                              /*找到并调度最高优先级就绪任务（执行OSSched()的时候会切换到最高优先级的就绪任务,由于当前任务已经从就绪列表中移
		除了，故是不会切换到当前这个任务的) */
   
						
		/* 当前任务（获得信号量）得以继续运行，执行到这里说明了当前任务从OSSched()处回来了，即是当前任务在释放信号量的OSSemPost()函数中被唤醒了，并且在执行任务切换的时候切换过来了，
			或者设置的阻塞时间timeout到了的时候自动苏醒过来  */
    CPU_CRITICAL_ENTER();                                   //关中断
						
    switch (OSTCBCurPtr->PendStatus) {                      //根据当前运行任务的等待状态分类处理
        case OS_STATUS_PEND_OK:                             //如果等待状态正常
             if (p_ts != (CPU_TS *)0) {                     //如果 p_ts 非空
                *p_ts  =  OSTCBCurPtr->TS;                  //获取信号被发布的时间戳
             }
            *p_err = OS_ERR_NONE;                           //返回错误类型为“无错误”
             break;

        case OS_STATUS_PEND_ABORT:                          
             if (p_ts != (CPU_TS *)0) {                     
                *p_ts  =  OSTCBCurPtr->TS;                 
             }
            *p_err = OS_ERR_PEND_ABORT;                     
             break;

        case OS_STATUS_PEND_TIMEOUT:                        
             if (p_ts != (CPU_TS *)0) {                     
                *p_ts  = (CPU_TS  )0;                        
             }
            *p_err = OS_ERR_TIMEOUT;                         
             break;

        case OS_STATUS_PEND_DEL:                            
             if (p_ts != (CPU_TS *)0) {                      
                *p_ts  =  OSTCBCurPtr->TS;                  
             }
            *p_err = OS_ERR_OBJ_DEL;                        
             break;

        default:                                          
            *p_err = OS_ERR_STATUS_INVALID;               
             CPU_CRITICAL_EXIT();                           //开中断
             return ((OS_SEM_CTR)0);                   
    }
    ctr = p_sem->Ctr;                                       //获取信号量的当前资源数目
    CPU_CRITICAL_EXIT();                                    //开中断
    return (ctr);                                           //返回信号量的当前资源数目
}