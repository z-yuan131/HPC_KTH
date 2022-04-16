#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define NSIZE 10000000
#define TRIALS 100	
#define MAXTHREADS 32
double x[NSIZE];
double sum_val, full_sum;
double localsum[MAXTHREADS];


void generate_random()
{
  int i;
  for (i = 0 ; i < NSIZE ; i++) {
      x[i] = rand() /(double) (RAND_MAX);
    }
}

void generate_known()
{
  int i;
  for (i=0; i< NSIZE; i++){
	x[i] = i+1;}
}

double serial_sum()
{
  int i;
  double sum_val= 0.0;
  for(i=0; i < NSIZE; i++){
    sum_val += x[i];
  }
  return sum_val;
}   

double for_sum()
{
  int i;
  double sum_val= 0.0;
  #pragma omp parallel for
  for(i=0; i < NSIZE; i++){
    sum_val += x[i];
  }
  return sum_val;
}


double critical_sum()
{
  int i;
  double sum_val= 0.0;
  #pragma omp critical
{
  for(i=0; i < NSIZE; i++){
    sum_val += x[i];
  }
}
  return sum_val;
}

int main(int argc, char* argv[]){
  //omp_set_num_threads(MAXTHREADS);
  int i,k,nthreads;
  double t1, t2, sum;
  #pragma omp parallel
{
  nthreads=omp_get_num_threads();
  #pragma omp single
  printf("number of threads=%d\n",nthreads);
}
  generate_random();
  //generate_known();
  serial_sum();

//serial version
  t1 = omp_get_wtime();
  for (i = 0; i < TRIALS; i++) {
    sum=serial_sum();
  }
  t2 = omp_get_wtime();
  printf("Sum_serial = %8.6f \n", sum); 
  printf("time_serial = %f\n", (t2 - t1)/(double)TRIALS);

//parallel for version
  t1 = omp_get_wtime();
  for (i = 0; i < TRIALS; i++) {
    sum=for_sum();
}
  t2 = omp_get_wtime();
  printf("Sum_para_for = %8.6f \n", sum); 
  printf("time_para_for = %f\n", (t2 - t1)/(double)TRIALS);

//parallel critical version
  t1 = omp_get_wtime();
  for (i = 0; i < TRIALS; i++) {   
    sum=critical_sum();
}
  t2 = omp_get_wtime();
  printf("Sum_critical = %8.6f \n", sum); 
  printf("time_critical = %f\n", (t2 - t1)/(double)TRIALS);

//local sum version
  t1 = omp_get_wtime();
  for (i=0 ; i < TRIALS; i++)
{
  for (k=0; k<nthreads; k++)
  localsum[k]=0.0;
  #pragma omp parallel
  {  
  int j;
  int id = omp_get_thread_num();
  //printf("id=%d\n", id);
  
  for (j = id; j < NSIZE ; j += nthreads)
    localsum[id] += x[j];
  }
  full_sum =0.0;
  for (k=0; k<nthreads; k++)
    full_sum += localsum[k];
}
  t2 = omp_get_wtime();
  printf("Sum_para_local = %8.6f \n", full_sum); 
  printf("time_para_local = %f\n", (t2 - t1)/(double)TRIALS);
  return 0;
}
