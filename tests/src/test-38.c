/*
 * test-38.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;
gtthread_mutex_t g_mutex;

void* thr1(void *in) {
  gtthread_mutex_lock(&g_mutex);
  gtthread_mutex_unlock(&g_mutex);
  printf("thr1 hello\n");
  return NULL;
}

int main() {
    printf("test5c. Should print 'main hello' then 'thr1 hello'.\n");

    gtthread_init(50000L);
    gtthread_mutex_init(&g_mutex);
    gtthread_mutex_lock(&g_mutex);

    gtthread_create( &t1, thr1, NULL);
    gtthread_yield();
    printf("main hello\n");
    gtthread_mutex_unlock(&g_mutex);
    gtthread_join(t1, NULL);
    return EXIT_SUCCESS;
}


