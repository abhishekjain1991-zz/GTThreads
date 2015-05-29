/*
 * test6.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "../../gtthread.h"
typedef struct argument {
  int countdown;
  int thread_number;
} args;

void *thr1(void *in) {
  args* arg = (args*)in;
  int thrNmbr = arg->thread_number;
  int cntDwn = arg->countdown;
  arg->thread_number++;
  if(arg->countdown-- > 0) {
    gtthread_t child;
    gtthread_create( &child, thr1, in);
    gtthread_join(child, NULL);
  }
  printf("thr%i: %i\n", thrNmbr, cntDwn);
  return NULL;
}

int main() {
    printf("Argument passing 2. Threads should print their number and a counter.\n Thread number must decrease while the number increases.\n");
    gtthread_t t1;
    args arg = {5, 1};
    gtthread_init(50000L);
    gtthread_create( &t1, thr1, &arg);
    gtthread_join(t1, NULL);

    return EXIT_SUCCESS;
}
