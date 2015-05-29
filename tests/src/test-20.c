/*
 * test9.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"
#include <unistd.h>

#define PTHREAD_CANCELED       ((void *)(size_t) -1)

void* thr1(void *in) {
    sleep(2);
    printf("Failure. This line should not be reached.\n");
    fflush(stdout);
    return NULL;
}

int main() {
    printf("Canceling a thread and joining it. Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_cancel(t1);
    void* result = NULL;
    if(gtthread_join(t1, &result)) {
      printf("Failure. Could not join canceled thread.\n");
    } else {
      if(result == PTHREAD_CANCELED) {
        printf("Success\n");
      } else {
        printf("Failure. Canceled threads join result was not as expected.\n");
      }
    }

    return EXIT_SUCCESS;
}
