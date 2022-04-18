#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[]){
  omp_set_num_threads(4);  //default is 8 on my mac. I assume each core a thread?
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    int np = omp_get_num_threads();
    printf("Hello world from thread %d of %d\n", id, np);
  }
  return 0;
}
