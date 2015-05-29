#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../gtthread.h"

/* Tests creation.
   Should print "Hello World!" */

void *thr1(void *in) {
    printf("Sucess\n");
    fflush(stdout);
    return NULL;
}

int main() {
    printf("Should print 'Sucess' and exit");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    gtthread_yield();

    return EXIT_SUCCESS;
}
