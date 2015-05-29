/*
 * test-33.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"
gtthread_t t;
void* thr(void *in) {
  if(gtthread_equal(gtthread_self(), t)) {
    printf("Success");
  } else {
    printf("Failure");
  }
  return NULL;
}

int main() {
  printf("test4c. Should print 'Success'.\n");

  gtthread_init(50000L);
  gtthread_create( &t, thr, NULL);
  gtthread_exit(NULL);
  return EXIT_SUCCESS;
}

