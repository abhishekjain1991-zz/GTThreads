/*
 * test8.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

gtthread_t main_thread;
void* thr1(void *in) {
  char* result;
  if(!gtthread_join(main_thread, (void**)&result)) {
    printf("%s\n",result);
  } else {
    printf("Failed to join.\n");
  }
  return NULL;
}

int main() {
    printf("Argument passing on join with exit. Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    main_thread = gtthread_self();
    gtthread_create( &t1, thr1, NULL);
    gtthread_exit("Success");

    return EXIT_SUCCESS;
}
