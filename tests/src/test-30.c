/*
 * test-30.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"
gtthread_t t1;
gtthread_t t2;

void* thr1(void *in) {
  int i = 0;
  for (i = 0; i < 1000; ++i)
  {
    int j = 0;
    for (j = 0; j < 10000; ++j);
  }
  printf("thr1 exiting...");
  return NULL;
}

void* thr2(void *in) {
  gtthread_join(t1, NULL);
  printf("thr2 exiting...");
  return NULL;
}



int main() {
    printf("test3d. Should print 'thr1 exiting...' then 'thr2 exiting...' then 'main exiting'.\n");

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_create( &t2, thr2, NULL);
    gtthread_join(t2, NULL);
    printf("main exiting");
    return EXIT_SUCCESS;
}

