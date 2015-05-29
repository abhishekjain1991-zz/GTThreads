// Test6
// gtthread_self. The program should print out liek below.
//
// same or diff
// diff    same

#include <stdio.h>
#include "../../gtthread.h"

gtthread_t g_th1, g_th2;

void* worker(void* arg)
{
	gtthread_t tid = gtthread_self();

	if(gtthread_equal(g_th2, tid))
		printf("same\n");
	else
		printf("diff\n");
        return NULL;
}

int main()
{
        printf("Should print \n'same'  or  'diff' \n 'diff'  or  'same'\n");
	int rc;

	gtthread_init(1000);

	rc = gtthread_create(&g_th1, worker, NULL);
        if(rc != 0) {
          printf("Could not create thread");
        }
	rc = gtthread_create(&g_th2, worker, NULL);
        if(rc != 0) {
          printf("Could not create thread 2");
        }

	gtthread_join(g_th1, NULL);
	gtthread_join(g_th2, NULL);
	return 0;
}
