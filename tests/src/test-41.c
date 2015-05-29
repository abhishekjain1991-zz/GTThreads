/*
 * test-41.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;
gtthread_mutex_t g_mutex;

void* thr1(void *in) {
  int i = 0;
  for (i = 0; i < 5; ++i) {
    gtthread_yield();
    printf("thr1 hello\n"); 
  }
  return NULL;
}

int main() {
    printf("test6a. Should print 'thr1 hello' five times.\n");

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_join(t1, NULL);
    return EXIT_SUCCESS;
}


