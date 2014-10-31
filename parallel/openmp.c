#include "stdio.h"
#include "omp.h"

int main(int argc, char const *argv[])
{

  int thread_id = 0;
  int number_of_loops = 0;
  
  #pragma omp parallel private(thread_id, number_of_loops) {

  #pragma omp for
    for (int i = 0; i < 1000; i++) {
      number_of_loops++;
    }

    thread_id = omp_get_thread_num();

    printf("Thread %d performed %d iterations.\n", thread_id, number_of_loops);

  }

  return 0;
}
