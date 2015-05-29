/*
 * test-35.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"

void* thr1(void *in) {
  return "Success";
}

int main() {
    printf("test4e. Should print 'Success'.\n");
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);
    char* result;
    int i = 0;
    for (i = 0; i < 10000; ++i) {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    if(!gtthread_join(t1, (void**)&result)) {
      printf("%s\n",result);
    } else {
      printf("Failed.\n");
    }

    return EXIT_SUCCESS;
}
