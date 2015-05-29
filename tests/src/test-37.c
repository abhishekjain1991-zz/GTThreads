/*
 * test-37.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;
gtthread_t t2;
gtthread_mutex_t g_mutex;

void* thr1(void *in) {
  gtthread_mutex_lock(&g_mutex);
  gtthread_mutex_unlock(&g_mutex);
  int k = 0;
  for (k = 0; k < 5; ++k) {
    int i = 0;
    for (i = 0; i < 10000; ++i) {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    gtthread_mutex_lock(&g_mutex);
    printf("thr1 hello\n");
    gtthread_mutex_unlock(&g_mutex);
  }
  return NULL;
}

void* thr2(void *in) {
  gtthread_mutex_lock(&g_mutex);
  gtthread_mutex_unlock(&g_mutex);
  int k = 0;
  for (k = 0; k < 5; ++k) {
    int i = 0;
    for (i = 0; i < 10000; ++i) {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    gtthread_mutex_lock(&g_mutex);
    printf("thr2 hello\n");
    gtthread_mutex_unlock(&g_mutex);
  }
  return NULL;
}


int main() {
    printf("test5b. Should print 'main hello' then 'thr1 hello' and 'thr2 hello' for 5 times in some order but not interleaving two prints (e.g 'th1 thr2 hello hello')\n");

    gtthread_init(50000L);
    gtthread_mutex_init(&g_mutex);
    gtthread_mutex_lock(&g_mutex);

    gtthread_create( &t1, thr1, NULL);
    gtthread_create( &t2, thr2, NULL);
    gtthread_yield();
    printf("main hello\n");
    gtthread_mutex_unlock(&g_mutex);
    gtthread_join(t2, NULL);
    gtthread_join(t1, NULL);
    return EXIT_SUCCESS;
}
