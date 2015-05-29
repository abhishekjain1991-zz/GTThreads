/*
 * test-27.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t main_thread;
void* thr1(void *in) {
    int i = 0;
    for (i = 0; i < 1000; ++i)
    {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    printf("Exit child thread.\n");
  return NULL;
}

int main() {
    printf("test3a. Should print 'Exit child thread.' then 'Exit main thread'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    int i = 0;
    for (i = 0; i < 10000; ++i)
    {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    printf("Exit main thread.\n");

    return EXIT_SUCCESS;
}
