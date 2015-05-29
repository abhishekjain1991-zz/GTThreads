/*
 * test-43.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;

void* thr1(void *in) {
  while(1) {
    printf("thr1 hi"); 
    gtthread_yield();
  }
  return NULL;
}

int main() {
    printf("test8a. Should print 'thr1 hi' one time.\n");

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_yield();
    gtthread_cancel(t1);
    gtthread_exit(NULL);
    return EXIT_SUCCESS;
}
