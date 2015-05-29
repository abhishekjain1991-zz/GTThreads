#include "gtthread.h"
#include<stdio.h>


#define PHILS 5

gtthread_t ph[PHILS];
gtthread_mutex_t mutex[PHILS];


void* phil(void*arg)
{	int r; 
	//who am i
	int my_id = (int)arg+1;
	//let's try to eat first
	//let me try to get 2 sticks based on the parameter passed
	//if my id is odd, i will get the stick with the 
	//index = my id first and then the one before with id 1 less than mine
	//if my index is even do the reverse so as to avoid any deadlock
	int first,second,j,seed;
	unsigned int eat_time,think_time;
	if(my_id%2)
	{
	first=my_id%PHILS;
	second=(my_id-1)%PHILS;
	}
	else
	{
	first=(my_id-1)%PHILS;
	second=my_id%PHILS;
	}
	//now let us eat first and then think and keep doing it
	while(1)
	{ 
		printf("Philosopher %d Hungry\n", my_id);
		
		gtthread_mutex_lock(&mutex[first]);
		printf("Philosopher %d Acquiring First Chopstick\n", my_id);
		gtthread_mutex_lock(&mutex[second]);
		printf("Philosopher %d Acquiring Second Chopstick\n", my_id);
		printf("Philosopher %d is eating \n",my_id);

		//lets eat for some random time
		eat_time=rand_r(&seed)%9999999;
		for(j=0; j < eat_time; j++);

		printf("Philosopher %d Done Eating\n", my_id);
		printf("Philosopher %d Releasing Second Chopstick\n", my_id);
		gtthread_mutex_unlock(&mutex[second]);
		printf("Philosopher %d Releasing First Chopstick\n", my_id);
		gtthread_mutex_unlock(&mutex[first]);
		printf("Philosopher %d is thinking \n",my_id);

		//Think randomly
		think_time=rand_r(&seed)%9999999;
		for(j=0; j < think_time; j++);

	}
	
}


int main()
{	printf("Let us begin the Dining Philosopher problem \n");


	gtthread_init(1);

	// Let us initialize the mutexes
	int i=0;
	for(i=0;i<PHILS;i++)
		gtthread_mutex_init(&mutex[i]);

	//Let the fun begin
	for(i=0;i<PHILS;i++)
		gtthread_create(&ph[i], phil, i);

	//MAN!!!! Main is out of here
	gtthread_exit(NULL);


}
