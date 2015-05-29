/*
 * test11.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

#define PTHREAD_CANCELED       ((void *)(size_t) -1)

gtthread_t mainthread;

void* thr1(void *in) {
    gtthread_cancel(mainthread);
    void* result = NULL;
    if(gtthread_join(mainthread, &result)) {
      printf("Failure. Could not join canceled thread.\n");
    } else {
      if(result == PTHREAD_CANCELED) {
        printf("Success\n");
      } else {
        printf("Failure. Canceled threads join result was not as expected.\n");
      }
    }
    return NULL;
}

int main() {
    printf("Thread wants to join another thread by which it is canceled and then joined.\n Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    mainthread = gtthread_self();
    gtthread_create( &t1, thr1, NULL);
    gtthread_join(t1, NULL);
    printf("Failure. Should not reach this code.\n");

    return EXIT_SUCCESS;
}
