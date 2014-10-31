#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

#define NUMBER_OF_THREADS 100000

void *func(void *void_ptr) {
  // cast void pointer to an int pointer,
  // then derefence for thread id value
  int thread_id = *((int *) void_ptr);
  printf("Thread id: %d\n", thread_id);

  return NULL;
}

int main(int argc, char const *argv[])
{

  pthread_t threads[NUMBER_OF_THREADS];
  int thread_ids[NUMBER_OF_THREADS];

  // spawn threads
  for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    thread_ids[i] = i;
    printf("Spawning thread: %d\n", i);
    pthread_create(&threads[i], NULL, func, (void *) &thread_ids[i]);
  }

  // join threads
  for (int i = 0; i < NUMBER_OF_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
