/*
 * test4.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

void *thr1(void *in) {
  int i = 10;
  for (i = 10; i > 0; --i) {
    printf("thr1: %i\n", i);
    gtthread_yield();
    gtthread_yield();
  }
  return NULL;
}

int main() {
    printf("Should count down on two threads one running at twice the speed as the other one.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    int i = 10;
    for (i = 10; i > 0; --i) {
      printf("thr0: %i\n", i);
      gtthread_yield();
    }
    gtthread_join(t1, NULL);

    return EXIT_SUCCESS;
}
