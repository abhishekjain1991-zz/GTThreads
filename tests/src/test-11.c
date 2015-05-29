/*
 * test0.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>
#include "../../gtthread.h"

void* thread(void* args) {
  long counter = 0;
  while((*(int*)args) > 0) {
    counter++;
    if((counter % 100000000) == 0) {
      (*(int*)args)--;
      printf("thread running... %i \n", (*(int*)args));
      counter = 0;
    }
  }
  (*(int*)args) = 10;
  return args;
}

int main() {
  printf("Run thread for 3 iterations.\n");
  printf("Then join and expect 10 as the return value.\n");
  gtthread_init(1000);
  gtthread_t test_thread;
  int iter = 3;
  gtthread_create(&test_thread, &thread, &iter);
  void* result;
  gtthread_join(test_thread, &result);
  printf("Thread returned %i\n", (*(int*)result));
  return EXIT_SUCCESS;
}
