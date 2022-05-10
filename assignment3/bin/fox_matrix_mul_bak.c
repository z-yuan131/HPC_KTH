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
double A_buff[MSIZE][MSIZE];
double B_buff[MSIZE][MSIZE];
double C[MSIZE][MSIZE];


void initialize_matrices(int Nsize)
{
  int i, j;

  for (i = 0 ; i < Nsize ; i++) {
    for (j = 0 ; j < Nsize ; j++) {
      A[i][j] = (double) rand() / RAND_MAX;
      B[i][j] = (double) rand() / RAND_MAX;
      C[i][j] = 0.0;
      //printf("%f,%f\n", A[i][j], B[i][j]);
    }
  }
}

void boardcast_matrices(int Nsize, int * dim, MPI_Group world_group, int * rank_ranges, int rank, int i){
  printf("%d,%d,%d\n", Nsize,dim[0],dim[1]);

  int root_rank;

  // first boardcast A matrix by creating groups row by row

  //for (int i = 0; i < dim[0]; i++)
  //{

    // create a new group each row to board cast A in row direction
    MPI_Group local_group;
    MPI_Group_incl(world_group, dim[1], rank_ranges, &local_group);

    // Get my rank in the odd group
    int my_local_group_rank;
    MPI_Group_rank(local_group, &my_local_group_rank);

    // Continue only if we are part of the odd group
    //if(my_local_group_rank != MPI_UNDEFINED)
    //{
    //    printf("I am process %d in the group and global rank is %d.\n", my_local_group_rank,rank);
    //}

    //int MPI_Bcast(void* buffer,
    //          int count,
    //          MPI_Datatype datatype,
    //          int emitter_rank,
    //          MPI_Comm communicator);
    // decide boardcast rank
    for (int j = 0; j < dim[0]; j++)
    {
      if (rank >= j*dim[1] && rank < (j+1)*dim[1])
      {
        root_rank = j + j*dim[1] + i;
        if (root_rank >= (j+1)*dim[1])
          root_rank -= dim[1];
      }
    }

    //if(my_local_group_rank != MPI_UNDEFINED)
    //{
    //    printf("I am local rank %d in the group and global rank is %d and my group root_rank is %d.\n", my_local_group_rank,rank,root_rank);
    //}

    // initialize A_buff with root_rank
    for (int j = 0; j < Nsize; j++){
      for (int k = 0; k < Nsize; k++){
        A_buff[j][k] = A[j][k];
      }
    }

    MPI_Bcast(&A_buff, (int)(Nsize)*Nsize, MPI_DOUBLE, root_rank, local_group);

    if (rank == 0 && rank == 1){
      for (int j = 0; j < Nsize; j++){
        for (int k = 0; k < Nsize; k++){
          printf("%f\n", A_buff[j][k]);
        }
      }
    }

    MPI_Group_free(&local_group);







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

  int dim[2], period[2], reorder, cart_rank, cart_size, coord[2], Nsize;

  int err = MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  if (err != MPI_SUCCESS){
    printf("Failed to initialize MPI process");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ((int)size / (int)(sqrt(size)) !=  (int)(sqrt(size))){
    printf("MPI process size must be a square number.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }


  dim[0] = (int)(sqrt(size)); //processors in x direction
  dim[1] = (int)(sqrt(size)); //processors in y direction
  period[0] = 0; //periodic(=ture) or not(=false)
  period[1] = 1; //periodic(=ture) or not(=false)
  reorder = 1;   //allow to reorder(=1)

  MPI_Comm comm_cart;           //comm_cart is communicator with new cartesian topology (handle)
  MPI_Cart_create(MPI_COMM_WORLD, 2, dim, period, reorder, &comm_cart);

  // Get the rank and size of the process
  MPI_Comm_size(comm_cart, &cart_size);
  MPI_Comm_rank(comm_cart, &cart_rank);


  // get coordinate of this rank
  MPI_Cart_coords(comm_cart, cart_rank, cart_size, coord);
  //printf("%d,%d,%d\n",cart_rank, coord[0], coord[1]);


  // get relationship between each rank
  int x_source[1], x_dest[1];
  MPI_Cart_shift(comm_cart, 0, 1, x_source, x_dest);
  int y_source[1], y_dest[1];
  MPI_Cart_shift(comm_cart, 1, 1, y_source, y_dest);

  //if (rank==4){
  //  printf("%d,%d,%d,%d\n", x_source[0],x_dest[0],y_source[0],y_dest[0]);
  //}

  // Get the group or processes of the default communicator
   MPI_Group world_group;
   MPI_Comm_group(MPI_COMM_WORLD, &world_group);

   // Get my rank in the world group
   int my_world_group_rank;
   MPI_Group_rank(world_group, &my_world_group_rank);

   // Selecting odd ranks means selecting every 2 ranks starting from 1, until the max rank <comm_size - 1>.

   int rank_ranges[1][dim[1]]; // = {1, size-1, 2};
   for (int i = 0; i < dim[0]; i++){
       if (i == coord[0]){
         for (int j = 0; j < dim[1]; j++)
          rank_ranges[0][j] = i*dim[1] + j;
       }
   }

   //printf("%d\n", rank_ranges[0][1]);










  //printf("1. Initializing Matrices \n");
  // init matrices with random numbers
  Nsize = (int)(MSIZE/(int)(sqrt(size)));
  initialize_matrices(Nsize);

  // matrices communication
  for (int i = 0; i < 1; i++){
    boardcast_matrices(Nsize,dim,world_group,(int *)rank_ranges,rank,i);
  }





  MPI_Finalize();

  return 0;
}
