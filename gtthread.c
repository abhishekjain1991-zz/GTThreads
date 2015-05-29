#include"gtthread.h"
#define MEM 640000
#define PTHREAD_CANCELED ((void *)(size_t) -1)
#define EDEADLK 35
#define MAX_THREADS 1000


//Handler for timer interrupts
void timer_event_handler (int signum)
{
	if(context_switch_permission)
	{
		current_context=current_context->next_context;
		gtthread_timer(timer_global_value);
		swapcontext(&(current_context->prev_context->thread_context),&(current_context->thread_context));
	}
}

//Initializes the timer
void gtthread_timer(long time)
{
	struct sigaction sa;
	struct itimerval timer;
	timer_global_value=time;
	// Install timer_handler as the signal handler for SIGVTALRM.
	memset (&sa, 0, sizeof (sa));
	sa.sa_flags=0;
	sa.sa_handler = &timer_event_handler;
	sigaction (SIGVTALRM, &sa, NULL);

	//Configure the timer
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = time;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = time;

	//Start the timer
	setitimer (ITIMER_VIRTUAL, &timer, NULL);

}

//Routine for proecess exit,scheduling and cleanup this allows me to clean up allocated memory
//if main calls gtthread_exit().
//Also, whenever a thread exits scheduler jumps to the node with this routine and then the scheduling continues

void exit_routine()
{
	
	while(exit_context->next_context!=exit_context)
	{	context_switch_permission=0;
		current_context=exit_context->next_context;
		context_switch_permission=1;
		gtthread_timer(timer_global_value);
		swapcontext(&exit_context->thread_context,&exit_context->next_context->thread_context);
	}

	//stop the timer from interrupting
	context_switch_permission=0;

	//do cleanup
	free(exit_context);
	free(status);


}


//***gtthread_init***/
//Creates the basic structure for the scheduler//
//also initializes and starts the timer
void gtthread_init(long time)
{
	//create a thread for exit routine and cleanup establish links properly
	exit_context=(gtthread*)malloc(sizeof(gtthread));
	if(exit_context==NULL)
		{
			printf("not able to start program \n");
			exit(0);
		}
	exit_context->next_context=exit_context;
	exit_context->prev_context=exit_context;
	getcontext(&exit_context->thread_context);
	exit_context->thread_context.uc_link=0;
	exit_context->thread_context.uc_stack.ss_sp=(char*)malloc(MEM);
	exit_context->thread_context.uc_stack.ss_size=MEM;
	exit_context->thread_context.uc_stack.ss_flags=0;
	exit_context->possess_lock=0;
	makecontext(&(exit_context->thread_context), (void*) (*exit_routine), 0);
	//create a context for main and establish links properly and update its properties
	main_context = (gtthread*)malloc(sizeof(gtthread));
	getcontext(&main_context->thread_context);
	if(main_context==NULL)
		{
			printf("not able to start program \n");
			exit(0);
		}
	main_context->thread_context.uc_link=0;
	main_context->thread_context.uc_stack.ss_sp=(char*)malloc(MEM);
	main_context->thread_context.uc_stack.ss_size=MEM;
	main_context->thread_context.uc_stack.ss_flags=0;
	main_context->next_context=exit_context;
	main_context->prev_context=exit_context;
	exit_context->next_context=main_context;
	exit_context->prev_context=main_context;
	main_context->possess_lock=0;
	tail=main_context;
	no_active_threads=0;
	main_context->thread_id=0;
	current_context=main_context;
	main_context->self=&main_context;
	//Make a status list for all threads
	status = (thread_list*)malloc(MAX_THREADS*sizeof(thread_list));
	if(status==NULL)
		{
			printf("not able to start program \n");
			exit(0);
		}
	//make status of main thread as running
	status[0].alive_status=1;
	status[0].finder=main_context;
	status[0].i_am_waiting_on=-1;
	//Calling my timer 
	context_switch_permission=1;
	gtthread_timer(time);
}

//***gtthread_exit***//
//used to delete a thread and delete the memory allocated for the thread on the heap 
void gtthread_exit(void *argument)
{
	context_switch_permission=0;
	temp_context=(current_context->self);
	status[(*temp_context)->thread_id].alive_status=0;
	status[(*temp_context)->thread_id].return_value=argument;
	status[(*temp_context)->thread_id].i_am_waiting_on=-1;

	//now delete the node
	(*temp_context)->prev_context->next_context=(*temp_context)->next_context;
 	(*temp_context)->next_context->prev_context=(*temp_context)->prev_context;
	
	free(*temp_context);
	context_switch_permission=1;
	gtthread_timer(timer_global_value);
	setcontext(&exit_context->thread_context);

}

//***gtthread_yield***//
//***Does the same thing as timer_event_handler***//
int gtthread_yield(void )
{
//jsut swap context to the next thread
	current_context=current_context->next_context;
	swapcontext(&(current_context->prev_context->thread_context),&(current_context->thread_context));
	return 0;
}
//***Every thread apart from main uses this routine to transfer the return values obtained on thread's routine termination to gtthread_exit***//
void start_with_gtthread_exit_routine(void* (*start_routine)(void*),void* arg)
{
//store value
	void *temp=(start_routine(arg));

//call exit to free node and go to exit_routine
	gtthread_exit(temp);
}

//***gtthread_create***//
//used to create a single thread //
int gtthread_create(gtthread_t *thread_id,void *(*start_routine)(void *),void *arg)
{ 
	context_switch_permission=0;
	gtthread **new_node;
	new_node=(gtthread*)malloc(sizeof(gtthread*));
	if (new_node==NULL)
		return -1;
	*new_node= ((gtthread*)malloc(sizeof(gtthread)));
	if (*new_node==NULL)
		return -1;
	//getting thread_id and assigning
	*thread_id=++no_active_threads;
	
	//establishing links between thread nodes
	(*new_node)->next_context=exit_context;
	(*new_node)->prev_context=tail;
	tail->next_context=(*new_node);
	tail=tail->next_context;
	exit_context->prev_context=tail;
	tail->thread_id=*thread_id;

	//changing alive status of thread
	status[tail->thread_id].alive_status=1;
	status[tail->thread_id].finder=*new_node;
	status[tail->thread_id].i_am_waiting_on=-1;
	tail->possess_lock=0;

	//assigning context values
	getcontext(&tail->thread_context);
	tail->thread_context.uc_link=0;
	tail->thread_context.uc_stack.ss_sp=(char*)malloc(MEM);
	tail->thread_context.uc_stack.ss_size=MEM;
	tail->thread_context.uc_stack.ss_flags=0;
	makecontext(&(tail->thread_context), (void*) (*start_with_gtthread_exit_routine), 2, start_routine,arg); 
	(*new_node)->self=new_node;
	context_switch_permission=1; 
	gtthread_timer(timer_global_value);

	return 0;
}


//***gtthread_join***//
//whichever thread calls this function waits until the thread that is specified in the argument for this routine terminates ***//
//implemented using single while loop//
//also has dealock detection and prevention in case 2 threads call join on each other with neither one of them exiting//
int gtthread_join(gtthread_t thread, void **ptr)
{
//printf("called for %d\n",thread);

	//update waiting on's  status
	status[current_context->thread_id].i_am_waiting_on=thread;

	//check for deadlock
	if(status[thread].i_am_waiting_on == current_context->thread_id)
		{	
			//possible deadlock detected
			status[current_context->thread_id].i_am_waiting_on=-1;
			return EDEADLK;
		}
		
	while(status[thread].alive_status==1)
	{
	}
	//clear flags for threads
	status[current_context->thread_id].i_am_waiting_on=-1;
	// if finished store value in ptr
	context_switch_permission=0;
	if(ptr!=NULL)
		*ptr = (void*)status[thread].return_value;
	context_switch_permission=1;
	gtthread_timer(timer_global_value);
	return 0;
}

//***gtthread_equal***//
//compares the thread ids specified as arguments and returns wheteher true or false//
int gtthread_equal(gtthread_t t1, gtthread_t t2)
{
	return t1==t2;
}
//***gtthread_self****//
//returns the thread_id of the caller thread//
gtthread_t gtthread_self(void)
{
	return current_context->thread_id;
}
//***gtthread_cancel***//
//cancels the thread specified by the argument//
int gtthread_cancel(gtthread_t thread_id)
{
	//done for deadlock detection and avoidance
	status[thread_id].i_am_waiting_on=-1;

	gtthread * temp_context2 = current_context;
	if(thread_id>no_active_threads || thread_id <0)
		{
			printf("no such threads exist");
			return -1;
		}
	//failure on trying to cancel a thread that has a lock
	if(status[thread_id].finder->possess_lock==1)
		{
			printf("Thread has lock, Cancel failed to avoid deadlock");
			return -1;
		}
	//if thread calls exit on itself exit the thread
	if(thread_id==current_context->thread_id)
	{
			gtthread_exit(PTHREAD_CANCELED);
			return 0;
	}
	//if current thread calls cancel for some other thread, change the context of the thread specified in argument
	//in such a way that next time the thread resumes it calls gtthread_exit on itself and terminates gracefully
	if(status[thread_id].alive_status)
		{
			makecontext(&(status[thread_id].finder->thread_context),(void*)(*gtthread_exit),1,PTHREAD_CANCELED);
			return 0;
		}
}
//***gtthread_mutex_init***///
//used to initialize a variable of type gtthread_mutex_t
int gtthread_mutex_init(gtthread_mutex_t *mutex)
{
	context_switch_permission=0;
	mutex=(gtthread_mutex_t*)malloc(1*sizeof(gtthread_mutex_t));
	if(mutex==NULL)
	{
		printf("unable to allocate memory for mutex\n");
		return -1;
	}
	mutex->context_holder=NULL;
	mutex->status=0;
	context_switch_permission=1;
	gtthread_timer(timer_global_value);
	return 0;
}
//***gtthread_mutex_lock***//
//***used to lock a mutex variable//
int gtthread_mutex_lock(gtthread_mutex_t *mutex)
{	//printf("blocked");
	//check if mutex is initially unlocked and if it is grant to the current context
	if(mutex->status==0)
	{	
		context_switch_permission=0;
		mutex->context_holder=current_context;
		mutex->count=1;
		mutex->status=1;
		current_context->possess_lock=1;
		context_switch_permission=1;
		gtthread_timer(timer_global_value);
		return 0;
	}
	//check if the holder is again asking for the lock or not
	else if(mutex->context_holder==current_context)
	{
		//if yes assuming it is a recursive lock grant the 
		//uncomment to use as recursive lock
		//context_switch_permission=0;
		//mutex->count++;
		//context_switch_permission=1;
		//gtthread_timer(timer_global_value);
		//uncomment above code and comment the code below to use lock as recursive lock
		return -1;
	}
	else if(mutex->context_holder!=current_context)
	{
		//if not, block!!
		while(mutex->status==1)
		{
		}
		//once released, grant lock
		context_switch_permission=0;
		mutex->context_holder=current_context;
		mutex->count=1;
		mutex->status=1;
		current_context->possess_lock=1;
		context_switch_permission=1;
		gtthread_timer(timer_global_value);
		return 0;
		
	}
	else
		return -1;
}
//***gtthread_mutex_unlock***//
//used to unlock the previously locked mutex variable//
int gtthread_mutex_unlock(gtthread_mutex_t *mutex)
{
	//check if mutex is unlocked
	if(mutex->status==0)
		return 0;
	else if(mutex->status)
		{
		 //check who is calling
		 if(current_context==mutex->context_holder)
			{
				//decrement count to see whether recursive lock or not
				--mutex->count;
				//if count is more than 0 it is recursive lock, do nothing
				//if(mutex->count>0);
				//if count ==0, then it is not recursive lock, release the context and flip the sataus
				//uncomment to use as recursive lock
				//else 
				if(mutex->count==0)
				{	context_switch_permission=0;
					mutex->status=0;
					mutex->context_holder=NULL;
					current_context->possess_lock=0;
					context_switch_permission=1;
					gtthread_timer(timer_global_value);
					return 0;
				}
			}
		else
			{//this is not allowed as a thread that did not lock the mutex_struct
			 //requested to unlock it_interval
			printf("Context of thread trying to acquire the lock is wrong \n");
			 return -1;
			} 
		}
}








