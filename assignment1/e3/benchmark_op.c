#define N 50000
#include <stdio.h>
#include <sys/time.h>

double mysecond();

int main(){
  int i, j;
  double t1, t2; // timers                                                         
  double a[N], b[N], c[N], d[2]; // arrays  
                                             
  // init arrays                                                                   
  for (i = 0; i < N; i++){
    a[i] = 47.0;
    b[i] = 3.1415;
  }

  // measure performance     
  // aviod cold start
  t1 = mysecond();
  for(i = 0; i < N; i++)
    c[i] = a[i]*b[i];
  c[0] = 1;
  t2 = mysecond();
  //printf("%f",t2-t1);

// opt
  double totalt;
  d[0] = 100;
  for (j = 0; j < 100; j++){
  t1 = mysecond();
  for(i = 0; i < N; i++)
    c[i] = a[i]*b[i];
  t2 = mysecond();
  d[1] = t2 - t1;
  if (d[1] < d[0]){d[0] = t2 -t1;}
  totalt += t2 - t1 ;
  }
  totalt = totalt/100;

  //printf("Execution time: %11.8f s\n", totalt);
  printf("%f",totalt);  
return 0;
}

// function with timer                                                             
double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}
