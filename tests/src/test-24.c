/*
 * test13.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t mainthread;

void* thr1(void *in) {
  printf("Join with thread0 returns: %i\n", gtthread_join(mainthread, NULL));
  return NULL;
}

int main() {
    printf("Thread trying to join each other.\n Should print result in 0 on one thread and != 0 (35 if EDEADLK) on the other one..\n");
    gtthread_t t1;

    gtthread_init(50000L);
    mainthread = gtthread_self();
    gtthread_create( &t1, thr1, NULL);
    printf("Join with thread1 returns: %i\n", gtthread_join(t1, NULL));
    return EXIT_SUCCESS;
}
