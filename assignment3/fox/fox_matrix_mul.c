#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921

#define TRIALS 10
#define MSIZE 128    //1024, a num which is pow of 2

double A[MSIZE][MSIZE];
double B[MSIZE][MSIZE];
double C[MSIZE][MSIZE];


void initialize_matrices()
{
  int i, j;

  for (i = 0 ; i < MSIZE ; i++) {
    for (j = 0 ; j < MSIZE ; j++) {
      A[i][j] = (double) rand() / RAND_MAX;
      B[i][j] = (double) rand() / RAND_MAX;
      C[i][j] = 0.0;
      printf("%f,%f\n", A[i][j], B[i][j]);
    }
  }
}


/*
double average_result()
{
  int i, j ;
  double  ave = 0.0;
  for (i = 0 ; i < MSIZE ; i++) {
    for (j = 0 ; j < MSIZE ; j++) {
      ave += matrix_r[i][j]/(double)(MSIZE*MSIZE);
    }
  }
  return(ave);
}*/

int main(int argc, char* argv[]){
  int rank, size, i, provided;

  int dim[2], period[2], reorder, cart_rank, cart_size, coord[1];

  int err = MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  if (err != MPI_SUCCESS){
    printf("Failed to initialize MPI process");
    return -1;
  }

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);





  dim[0] = (int)(sqrt(size)); //processors in x direction
  dim[1] = (int)(sqrt(size)); //processors in y direction
  period[0] = 0; //periodic(=ture) or not(=false)
  period[1] = 0; //periodic(=ture) or not(=false)
  reorder = 1;   //allow to reorder(=1)

  MPI_Comm comm_cart;           //comm_cart is communicator with new cartesian topology (handle)
  MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm_cart);

  // Get the rank and size of the process
  MPI_Comm_rank(comm_cart, &cart_rank);
  MPI_Comm_size(comm_cart, &cart_size);


  // get coordinate of this rank
  MPI_Cart_coords(comm_cart, cart_rank, cart_size, coord);
  //printf("%d,%d\n",cart_rank, coord[0]);

  // get relationship between each rank
  int x_source[1], x_dest[1];
  MPI_Cart_shift(comm_cart, 0, 1, x_source, x_dest);
  int y_source[1], y_dest[1];
  MPI_Cart_shift(comm_cart, 1, 1, y_source, y_dest);

  if (rank==3){
    printf("%d,%d,%d,%d\n", x_source[0],x_dest[0],y_source[0],y_dest[0]);
  }




  //printf("1. Initializing Matrices \n");
  // init matrices with random numbers
  initialize_matrices();



  MPI_Finalize();

  return 0;
}
