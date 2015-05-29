/*
 * test12.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

#define PTHREAD_CANCELED       ((void *)(size_t) -1)

gtthread_t main_thread;
void* thr1(void *in) {
  char* result;
  gtthread_yield();
  gtthread_cancel(main_thread);
  if(!gtthread_join(main_thread, (void**)&result)) {
    if(result != PTHREAD_CANCELED) {
      printf("%s\n", result);
    } else {
      printf("Did return PTHREAD_CANCELED. This is an error.\n");
    }
  } else {
    printf("Failed to join.\n");
  }
  return NULL;
}

int main() {
    printf("Canceling a thread that is already exited and joining it afterwards.\n Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    main_thread = gtthread_self();
    gtthread_create( &t1, thr1, NULL);
    gtthread_exit("Success");

    return EXIT_SUCCESS;
}
