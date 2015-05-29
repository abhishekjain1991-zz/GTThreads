/*
 * test5.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

void *thr1(void *in) {
  printf("%s\n", (char*)in);
  return NULL;
}

int main() {
    printf("Passing argument. Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, "Success");
    gtthread_join(t1, NULL);

    return EXIT_SUCCESS;
}
