#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]){
  omp_set_num_threads(5);  //default is 8 on my mac. I assume each core a thread?
  #pragma omp parallel
  {
    for (int i = 0; i < 2; i++){
      for (int k = 0; k < 2; k++){
        #pragma omp for
        for (int j = 0; j < 5; j++){
          int id = omp_get_thread_num();
          int np = omp_get_num_threads();
          printf("Hello world from thread %d of %d\n", id, np);
        }
      }

    }

  }
  return 0;
}
