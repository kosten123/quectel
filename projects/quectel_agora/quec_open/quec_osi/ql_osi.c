/**
 * @file ql_osi_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <quec_osi/ql_osi.h>

QlOSStatus ql_rtos_task_create(ql_task_t *taskRef, uint32 stackSize, uint8 priority, char *taskName, 
				void (*taskStart)(void*), void* argv)
{
	return rtos_create_thread((beken_thread_t *)taskRef, priority, taskName, (beken_thread_function_t)taskStart, stackSize, argv);
	
}

 QlOSStatus ql_rtos_task_delete
(
	ql_task_t taskRef		/* OS task reference	*/
)
{
	return rtos_delete_thread((beken_thread_t *)&taskRef);

}

QlOSStatus ql_rtos_task_suspend
(
	ql_task_t taskRef		/* OS task reference	*/
)
{
	 rtos_suspend_thread((beken_thread_t *)&taskRef);
	 return 0;
}

QlOSStatus ql_rtos_task_resume
(
	ql_task_t taskRef		/* OS task reference	*/
)
{
	if (taskRef == NULL)
    {
        vTaskResume(NULL);
	}
    else
    {
        vTaskResume((TaskHandle_t)(taskRef));
	}
	return 0;
}


 QlOSStatus ql_rtos_task_get_current_ref
(
	ql_task_t * taskRef		/* OS task reference	*/
)
{
	taskRef = rtos_get_current_thread();
	return 0;
}

 QlOSStatus ql_rtos_task_change_priority
(
	ql_task_t 	taskRef,	
	uint8 		new_priority		
)
{
    if ( new_priority > RTOS_HIGHEST_PRIORITY )
    {
        new_priority = RTOS_HIGHEST_PRIORITY;
    }

	vTaskPrioritySet(taskRef, BK_PRIORITY_TO_NATIVE_PRIORITY(new_priority));
	return 0;
}

 void ql_rtos_task_sleep_ms
(
	uint32 ms	   /* OS task sleep time for ms */
)
{
	rtos_delay_milliseconds(ms);
}
	

void ql_rtos_task_sleep_s
(
   uint32 ms	  /* OS task sleep time for ms */
)
{
   rtos_delay_milliseconds(ms*1000);
}



 QlOSStatus ql_rtos_enter_critical(void)
 {
	 rtos_enter_critical();
	 return 0;
 
 }
 
 QlOSStatus ql_rtos_exit_critical(void)
 {
	rtos_exit_critical();
	return 0;
 }

#if 1
 QlOSStatus ql_rtos_semaphore_create
(
	ql_sem_t	*semaRef,		/* OS semaphore reference						*/
	uint32		initialCount	/* initial count of the semaphore				*/
)
{
	return rtos_init_semaphore(semaRef, initialCount);
}

 QlOSStatus ql_rtos_semaphore_delete
(
	ql_sem_t   semaRef        /* OS semaphore reference                  		*/
)
{
	return rtos_deinit_semaphore((beken_semaphore_t *)&semaRef);
}


 QlOSStatus ql_rtos_semaphore_release
(
	ql_sem_t   semaRef        /* OS semaphore reference						*/
)
{
	return rtos_set_semaphore((beken_semaphore_t *) &semaRef);
}

 QlOSStatus ql_rtos_semaphore_get_cnt
(
	ql_sem_t  	semaRef,       /* OS semaphore reference           				*/
	uint32    * cnt_ptr    	   /* out-parm to save the cnt of semaphore      	*/
)
{
	*cnt_ptr =  rtos_get_sema_count((beken_semaphore_t*)&semaRef);
	return QL_OSI_SUCCESS;
}


 QlOSStatus ql_rtos_semaphore_wait(ql_sem_t semaRef, uint32 timeout)
{
	return rtos_get_semaphore((beken_semaphore_t*)&semaRef, timeout);
}

 QlOSStatus ql_rtos_mutex_create
(
    ql_mutex_t  *mutexRef        /* OS mutex reference                         */
)
{
	return rtos_init_mutex((beken_mutex_t *)mutexRef);
}

 QlOSStatus ql_rtos_mutex_lock
(
	ql_mutex_t    mutexRef       /* OS mutex reference                         */
)
{
	return rtos_lock_mutex((beken_mutex_t *) &mutexRef);
}

 QlOSStatus ql_rtos_mutex_unlock
(
	ql_mutex_t  mutexRef        /* OS mutex reference                         */
)
{
	return rtos_unlock_mutex((beken_mutex_t *) &mutexRef);
}

 QlOSStatus ql_rtos_mutex_delete
(
	ql_mutex_t  mutexRef        /* OS mutex reference                         */
)
{
	return rtos_deinit_mutex((beken_mutex_t *) &mutexRef);
}
 QlOSStatus ql_rtos_queue_create
(
	ql_queue_t   	*msgQRef,       	/* OS message queue reference              */
	uint32         	maxSize,        	/* max message size the queue supports     */
	uint32         	maxNumber	      	/* max # of messages in the queue          */
)
{
	return rtos_init_queue((beken_queue_t*)msgQRef, NULL, maxSize, maxNumber);
}

 QlOSStatus ql_rtos_queue_wait
(
	ql_queue_t   	msgQRef,		/* message queue reference                 		*/
	uint8  	      	*recvMsg,       /* pointer to the message received         		*/
	uint32         	size, 			/* size of the message                     		*/
	uint32         	timeout         /* QL_WAIT_FOREVER, QL_NO_WAIT, or timeout  */
)
{
	return rtos_pop_from_queue((beken_queue_t*) &msgQRef, recvMsg, timeout);
}
QlOSStatus ql_rtos_queue_release
(
    ql_queue_t		msgQRef,        /* message queue reference                 			*/
    uint32         	size,           /* size of the message                     			*/
    uint8          	*msgPtr,        /* start address of the data to be sent    			*/
    uint32         	timeout         /* QL_WAIT_FOREVER, QL_NO_WAIT, or timeout   	*/
)
{
	return rtos_push_to_queue((beken_queue_t*) &msgQRef, msgPtr, timeout);
}

 QlOSStatus ql_rtos_queue_delete
(
	ql_queue_t	msgQRef         /* message queue reference                 		*/
)
{
	return rtos_deinit_queue((beken_queue_t*) &msgQRef);
}
 QlOSStatus ql_rtos_timer_create
(
	ql_timer_t	   * timerRef,					/* OS supplied timer reference	*/
	uint32_t 		time_ms,
	void		   (*callBackRoutine)(void *),	   /* timer call-back routine						   */
	void		   *timerArgc				   /* argument to be passed to call-back on expiration */
)
{
	return rtos_init_timer((beken_timer_t*)timerRef, time_ms, callBackRoutine, timerArgc);
}

 QlOSStatus ql_rtos_timer_start
(
	ql_timer_t		timerRef					/* OS supplied timer reference						*/
)
{
	return rtos_start_timer((beken_timer_t*) &timerRef);
}
 QlOSStatus ql_rtos_timer_stop
(
	ql_timer_t timerRef 				/* OS supplied timer reference	*/
)
{
	return rtos_stop_timer((beken_timer_t*) &timerRef);
}

 QlOSStatus ql_rtos_timer_is_running
(
	ql_timer_t timerRef					/* OS supplied timer reference	*/
)
{
	return rtos_is_timer_running((beken_timer_t*) &timerRef);
}
 QlOSStatus ql_rtos_timer_delete
(
	ql_timer_t timerRef 				/* OS supplied timer reference	*/
)
{
	return rtos_deinit_timer((beken_timer_t*) &timerRef);
}


uint32_t ql_rtos_up_time_ms()
{
	return rtos_get_time();
}

QlOSStatus ql_rtos_timer_change_period
(
  ql_timer_t timerRef,
  uint32_t 	time_ms
)
{
	return  rtos_change_period((beken_timer_t*)&timerRef,time_ms);
}
#endif
