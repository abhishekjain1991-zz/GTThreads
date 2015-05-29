/*
 * test-45.c
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
    printf("test8b. Should print 'thr1 hi' until stopped.\n");

    gtthread_init(5000000000L);
    gtthread_create( &t1, thr1, NULL);
    while(1) {
      gtthread_yield();
    }
    gtthread_exit(NULL);
    return EXIT_SUCCESS;
}
