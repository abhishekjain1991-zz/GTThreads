/*
 * test-42.c
 */

#include <stdio.h>
#include "../../gtthread.h"

gtthread_mutex_t g_mutex;

void* worker(void* arg)
{
  gtthread_t th2;
  if(arg != NULL) {
    gtthread_create(&th2, worker, NULL);
  } else {
    arg = (void*)2;
  }
  int i = 0;

  for(i=0; i < 10; i++) {
    printf("Hi: %ld\n", (long)arg);
    int k = 0;
    for (k = 0; k < 10000; ++k) {
      int j = 0;
      for (j = 0; j < 10000; ++j);
    }
    gtthread_yield();
  }
  if((long)arg == 1) {
    gtthread_join(th2, NULL);
  }
  return NULL;
}

int main() {
    printf("test7. Should print 'Hi: 1' and 'Hi: 2' ten times, interleaving somehow.\n");
  gtthread_t th1;

  gtthread_init(1000);

  gtthread_mutex_init(&g_mutex);

  gtthread_create(&th1, worker, (void*)1);

  gtthread_join(th1, NULL);
  return 0;
}
