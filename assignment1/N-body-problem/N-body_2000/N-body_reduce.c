// N-body problem
#define N 2000
#include <stdio.h>
//#include <sys/time.h>
#include <string.h> /* For memset */
#include <stdlib.h>
#include <math.h>

#define DIM 2
typedef double vect_t[DIM];

/* function declaration */
void compute_force(int n, double G, int q, vect_t *force, double *mass, vect_t *pos);


int main(int argc, char *argv[]) {
    int T, n = N, nstep = 100;   //n: number of particles
    double dt, G = 1;//G = 6.673e-11;


    if (argc == 3) {
      dt = atof(argv[1]); //atoi asciII to float
      T = atoi(argv[2]);  //atoi asciII to int
      printf("input values: T = %d, dt = %f \n",T,dt);
    }
    else {printf("You did not feed me (enough) arguments, I will die now :( ...\n"); exit(1);}

    // initialization of the code
    vect_t *force = malloc(n * sizeof(vect_t));
    vect_t *pos   = malloc(n * sizeof(vect_t));
    vect_t *vel   = malloc(n * sizeof(vect_t));
    double *mass  = malloc(n * sizeof(double));

    //force = memset(force, 0, n*sizeof(vect_t));

    for (int q = 0; q < n; q++) {
  	   pos[q][0] = (rand() / (double)(RAND_MAX)) * 2 - 1;
  	   pos[q][1] = (rand() / (double)(RAND_MAX)) * 2 - 1;

  	   vel[q][0] = (rand() / (double)(RAND_MAX)) * 2 - 1;
  	   vel[q][1] = (rand() / (double)(RAND_MAX)) * 2 - 1;

  	   mass[q] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);

       //printf("%f", (rand() / (double)(RAND_MAX)) * 2 - 1);
    }
    //printf("%f,%d\n",dt,T);


    // main program
    for (int step = 0; step < nstep; step++){
      force = memset(force, 0, n*sizeof(vect_t));
      for (int q = 0; q < n; q++){
        
        compute_force(n, G, q, force, mass, pos);



        //paticle mover
        pos[q][0] += dt*vel[q][0];
        pos[q][1] += dt*vel[q][1];
        vel[q][0] += dt/mass[q]*force[q][0];
        vel[q][1] += dt/mass[q]*force[q][1];


      }
      //exit(1);
    }

    return 0;
}


void compute_force(int n, double G, int q, vect_t *force, double *mass, vect_t *pos){
  //printf("%f\n", mass[q]);
  double dist_cube, x_diff, y_diff, forceqk[2];
  for (int p = 0; p < n; p++){
    if (p > q){
      x_diff = pos[q][0] - pos[p][0];
      y_diff = pos[q][1] - pos[p][1];
      dist_cube = pow(sqrt(x_diff * x_diff + y_diff * y_diff) , 3);

      forceqk[0] = G*mass[q]*mass[p]/dist_cube * x_diff;
      forceqk[1] = G*mass[q]*mass[p]/dist_cube * y_diff;


      force[q][0] += forceqk[0];
      force[q][1] += forceqk[1];
      force[p][0] -= forceqk[0];
      force[p][1] -= forceqk[1];
    }
  }
  //return force;
}
    //
