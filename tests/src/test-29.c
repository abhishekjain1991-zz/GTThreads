/*
 * test-29.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

void* thr1(void *in) {
    int i = 0;
    for (i = 0; i < 1000; ++i)
    {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    printf("thr1 hello\n");
  return NULL;
}
void* thr2(void *in) {
    int i = 0;
    for (i = 0; i < 1000; ++i)
    {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    printf("thr2 hello\n");
  return NULL;
}



int main() {
    printf("test3c. Should print 'thr1' and 'thr2' interleaving somehow.\n");
    gtthread_t t1;
    gtthread_t t2;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_create( &t2, thr2, NULL);
    gtthread_exit(NULL);
    return EXIT_SUCCESS;
}
