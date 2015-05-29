/*
 * test2.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"
#include <unistd.h>

void *thr1(void *in) {
    printf("Success\n");
    fflush(stdout);
    return NULL;
}

int main() {
    printf("Should print 'Success', then the main thread joins and exits.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_join(t1, NULL);

    return EXIT_SUCCESS;
}
