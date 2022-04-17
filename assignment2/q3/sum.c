#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define MAX_THREADS 2

typedef struct sum_opt{
  double sum;
  char pad[128];
} sum_opt[MAX_THREADS];

void generate_random(double *input, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    input[i] = rand() / (double)(RAND_MAX);
  }
}

double serial_sum(double *x, size_t size)
{
  double sum_val = 0.0;

  double time = omp_get_wtime();
  for (size_t i = 0; i < size; i++) {
    sum_val += x[i];
  }

  printf("serial time is %f \n", omp_get_wtime() - time);

  return sum_val;
}


double omp_sum(double *x, size_t size)
{
  double sum_val = 0.0;

  omp_set_num_threads(MAX_THREADS);  //default is 8 on my mac.
  double time = omp_get_wtime();
  #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
      sum_val += x[i];
    }


  printf("parallel time is %f \n", omp_get_wtime() - time);

  return sum_val;
}


double omp_critical_sum(double *x, size_t size)
{
  double sum_val = 0.0;

  omp_set_num_threads(MAX_THREADS);  //default is 8 on my mac.
  double time = omp_get_wtime();
  #pragma omp parallel for
    for (size_t i = 0; i < size; i++) {
      #pragma omp critical
      {
        sum_val += x[i];
      }
    }


  printf("critical parallel time is %f \n", omp_get_wtime() - time);

  return sum_val;
}


double omp_local_sum(double *x, size_t size)
{
  omp_set_num_threads(MAX_THREADS);  //default is 8 on my mac.
  double sum_all[MAX_THREADS], sum_val = 0.0;

  double time = omp_get_wtime();
  #pragma omp parallel shared(sum_all)
  {

    size_t id = omp_get_thread_num();

    #pragma omp for
      for (size_t i = 0; i < size; i++) {
          sum_all[id] += x[i];
      }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    sum_val += sum_all[i];
  }


  printf("local parallel time is %f \n", omp_get_wtime() - time);

  return sum_val;


}



double omp_opt_local_sum(double *x, size_t size)
{
  omp_set_num_threads(MAX_THREADS);  //default is 8 on my mac.
  double sum_val = 0.0;
  sum_opt opt_sum;

  double time = omp_get_wtime();
  #pragma omp parallel shared(opt_sum)
  {

    size_t id = omp_get_thread_num();

    #pragma omp for
      for (size_t i = 0; i < size; i++) {
          opt_sum[id].sum += x[i];
      }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    sum_val += opt_sum[i].sum;
  }


  printf("local parallel opt time is %f \n", omp_get_wtime() - time);

  return sum_val;


}



int main(){

  double input[N], sum_s, sum_p1, sum_p2, sum_p3, sum_p4;


  generate_random(input, N);



  // serail part
  sum_s = serial_sum(input, N);
  printf("serial result is %f \n",sum_s);

  // parallel part 1:
  sum_p1 = omp_sum(input, N);
  printf("omp for result is %f \n",sum_p1);

  // parallel part 2:
  sum_p2 = omp_critical_sum(input, N);
  printf("omp critical result is %f \n",sum_p2);

  // parallel part 3:
  sum_p3 = omp_local_sum(input, N);
  printf("omp local result is %f \n",sum_p3);

  // parallel part 4:
  sum_p4 = omp_opt_local_sum(input, N);
  printf("omp local opt result is %f \n",sum_p4);
  return 0;
}
