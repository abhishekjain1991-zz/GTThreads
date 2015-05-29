/*
 * test-44.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t t1;
gtthread_mutex_t g_mutex;

void* thr1(void *in) {
  gtthread_mutex_lock(&g_mutex);
  gtthread_mutex_unlock(&g_mutex);
  printf("failure");
  return NULL;
}

int main() {
    printf("test6d. The program should either hang or exit but not print anything (kill it using your task manager).\n");

    gtthread_init(50000L);
    gtthread_mutex_init(&g_mutex);
    gtthread_mutex_lock(&g_mutex);
    gtthread_create( &t1, thr1, NULL);
    gtthread_join(t1, NULL);
    printf("failure.");
    return EXIT_SUCCESS;
}


