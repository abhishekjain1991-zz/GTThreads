#include <stdlib.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <ucontext.h>
#include <stdio.h>
#include <string.h> 
#include <unistd.h> 

// My thread structure
typedef struct thread
{
ucontext_t thread_context;	//used to store context of current thread
int thread_id;			//used to store id of current thread
struct thread* next_context;	//used to store link to next thread for round robin scheduling
struct thread* prev_context;	//used to store link to previous thread for round robin scheduling
struct thread** self;		//used to store a pointer to itself so as to remove errors while using free
int possess_lock;		//used to checck if a thread holds a lock before it is cancelled and thus prevent deadlock
}gtthread;


//a record for all threads
typedef struct node_record
{
void *return_value;		//used to store the return value that the the thread returns
int alive_status;		//used to indicate whether thread is alive or dead
gtthread *finder;		//used to refernce the node of gtthread
int i_am_waiting_on;		//used to indicate who the current thread is waiting on to avoid deadlock
}thread_list;

typedef struct mutex_struct
{
gtthread *context_holder; 	// to detect who actually has the lock
int count;  			//to be used in case of recursive mutex
int status;			//0 or 1 for holding lock
}gtthread_mutex_t;

//create a pointer for the status list

thread_list *status;

// create pointer for main thread,tail and current_context

gtthread *main_context,*tail,*current_context,**temp_context,*exit_context;



// Value in microseconds for the timer routine

int my_time;


//Start the timmer

void gtthread_timer(long time);

//Keep a count of the number of active threads

int no_active_threads;

// init function for creating a context for the main node and start scheduling

void gtthread_init(long time);

//gt_thread_t as a substitute for pthread_t

typedef int gtthread_t;

//gtthread join

int  gtthread_join(gtthread_t thread, void **status);

//gtthread exit

void gtthread_exit(void *retval);

//gtthread yield
int  gtthread_yield(void);

//gtthread equal
int  gtthread_equal(gtthread_t t1, gtthread_t t2);

//gtthread cancel
int  gtthread_cancel(gtthread_t thread);

//gtthread self
gtthread_t gtthread_self(void);

// Mutex functions

//Initialize a mutex object
int gtthread_mutex_init(gtthread_mutex_t *mutex);

//Lock a mutex
int gtthread_mutex_lock(gtthread_mutex_t *mutex);

//Unlock a mutex
int gtthread_mutex_unlock(gtthread_mutex_t *mutex);

//Since blocking unblocking is not working that well, use a global variable to block temporary context switch
int context_switch_permission;

// Create a thread
int gtthread_create(gtthread_t *thread_id,void *(*start_routine)(void *),void *arg);

//Timer global variable
int timer_global_value;
