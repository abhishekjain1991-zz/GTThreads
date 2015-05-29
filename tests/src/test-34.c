/*
 * test-34.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;
gtthread_t t2;

void* thr1(void *in) {
  if(gtthread_equal(gtthread_self(), t1)) {
    printf("Success\n");
  } else {
    printf("Failure\n");
  }
  return NULL;
}

void* thr2(void *in) {
  if(gtthread_equal(gtthread_self(), t1)) {
    printf("Failure.\n");
  } else {
    printf("Success\n");
  }
  return NULL;
}


int main() {
    printf("test4d. Should print 'Success' three times.\n");

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_create( &t2, thr2, NULL);
    gtthread_join(t2, NULL);
    gtthread_join(t1, NULL);
    printf("Success\n");
    return EXIT_SUCCESS;
}

