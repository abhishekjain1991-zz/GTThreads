/*
 * test3.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t main_thread;

void *thr1(void *in) {
    gtthread_join(main_thread, NULL);
    printf("Success\n");
    fflush(stdout);
    return NULL;
}

int main() {
    printf("Should print 'Success', then exit.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    main_thread = gtthread_self();
    gtthread_create( &t1, thr1, NULL);
    gtthread_exit(NULL);
    return EXIT_SUCCESS;
}
