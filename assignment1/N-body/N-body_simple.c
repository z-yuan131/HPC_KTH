// N-body problem
#define N 5000
#include <stdio.h>
//#include <sys/time.h>
#include <string.h> /* For memset */
#include <stdlib.h>

#define DIM 2
typedef double vect_t[DIM];


int main(int argc, char *argv[]) {
    int T;
    double dt;
    
    if (argc == 3) {dt = atof(argv[1]); T = atoi(argv[2]);}  //atoi asciII to int
    else {printf("You did not feed me arguments, I will die now :( ..."); exit(1);}
    
    printf("%f,%d",dt,T);
    return 0;
}
