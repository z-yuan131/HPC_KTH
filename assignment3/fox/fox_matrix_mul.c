#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MSIZE 5000    //1024, a num which is pow of 2

#define Trials 10

double A[MSIZE][MSIZE];
double B[MSIZE][MSIZE];
double A_buff[MSIZE][MSIZE];
double B_buff[MSIZE][MSIZE];
double C[MSIZE][MSIZE];
double temp[MSIZE][MSIZE];


void initialize_matrices(int Nsize)
{
  int i, j;

  for (i = 0 ; i < Nsize ; i++) {
    for (j = 0 ; j < Nsize ; j++) {
      A[i][j] = (double) rand() / RAND_MAX;
      B[i][j] = (double) rand() / RAND_MAX;
      C[i][j] = 0.0;
      temp[i][j] = 0.0;
      //printf("%f,%f\n", A[i][j], B[i][j]);
    }
  }
}

void boardcast_matrices(int Nsize, int * dim, MPI_Comm row_comm,MPI_Comm comm_cart, int rank, int row_rank, int i, int y_source, int y_dest){
  //printf("%d,%d,%d\n", Nsize,dim[0],dim[1]);

  int root_rank;

  // initialize A_buff with root_rank
  for (int j = 0; j < Nsize; j++){
    for (int k = 0; k < Nsize; k++){
      A_buff[j][k] = A[j][k];
    }
  }

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
      root_rank = root_rank % dim[1];

      //  printf("I am row rank %d and global rank is %d and my root_rank is %d and comm root rank is %d.\n", row_rank,rank,root_rank,root_rank);
    }
  }

  // boardcast A in row direction
  MPI_Bcast(&A_buff, (int)(Nsize*Nsize), MPI_DOUBLE, root_rank, row_comm);

  // get B from y direction neighbours
  //MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
  //             int dest, int sendtag,
  //             void *recvbuf, int recvcount, MPI_Datatype recvtype,
  //             int source, int recvtag, MPI_Comm comm, MPI_Status * status)

  // at the first iteration, this opration is not needed.
  if (i != 0){
    MPI_Sendrecv(&B,      (int)(Nsize*Nsize), MPI_DOUBLE, y_source,   1,
                 &B_buff, (int)(Nsize*Nsize), MPI_DOUBLE, y_dest,     1, comm_cart, MPI_STATUS_IGNORE);
  }
}


void calculate_C(int Nsize)
{
    for (int i = 0; i < Nsize; i++){
      for (int j = 0; j < Nsize; j++) {
          C[i][j] += A[i][j]*B[i][j];
      }
    }
}


void average_result(int Nsize, int rank, int size){
  //int MPI_Reduce(const void* send_buffer,
  //             void* receive_buffer,
  //             int count,
  //             MPI_Datatype datatype,
  //             MPI_Op operation,
  //             int root,
  //             MPI_Comm communicator);
  MPI_Reduce(&C, &temp, (int)(Nsize*Nsize), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  double avg = 0;
  if (rank == 0){
    for (int i = 0; i < Nsize; i++){
      for (int j = 0; j < Nsize; j++){
        avg += temp[i][j] / (double)(Nsize*Nsize);
      }
    }
    avg = avg / size;
    printf("summation is %f\n", avg);
  }
}

int main(int argc, char* argv[]){
  int rank, size, i, provided;

  int dim[2], period[2], reorder, cart_rank, cart_size, coord[2], Nsize;

  int err = MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  if (err != MPI_SUCCESS){
    printf("Failed to initialize MPI process");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }


  double start_time, stop_time, elapsed_time;
  start_time = MPI_Wtime();




  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if ((int)size / (int)(sqrt(size)) !=  (int)(sqrt(size))){
    printf("MPI process size must be a square number.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }


  // construct one communicator with cartitan coordinate
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


  // ---------------------------------------------------------
  // another communicator in row direction
  int color = rank / dim[0]; // Determine color based on row
  //printf("%d,%d\n", rank,color);

  // Split the communicator based on the color and use the
  // original rank for ordering
  MPI_Comm row_comm;
  MPI_Comm_split(MPI_COMM_WORLD, color, rank, &row_comm);

  int row_rank, row_size;
  MPI_Comm_rank(row_comm, &row_rank);
  MPI_Comm_size(row_comm, &row_size);



  //printf("1. Initializing Matrices \n");
  // init matrices with random numbers
  Nsize = (int)(MSIZE/(int)(sqrt(size)));


  initialize_matrices(Nsize);

  // main solver
  for (int trial = 0; trial < Trials; trial++){
    for (int i = 0; i < dim[1]; i++){
      boardcast_matrices(Nsize,dim,row_comm,comm_cart,rank,row_rank,i,y_source[0],y_dest[0]);
      calculate_C(Nsize);
    }
  }



  MPI_Comm_free(&row_comm);
  MPI_Comm_free(&comm_cart);


  stop_time = MPI_Wtime();
  elapsed_time = stop_time - start_time;

  average_result(Nsize, rank, size);


  if (rank == 0)
  {
    printf("Final elapsed time is %f s\n", elapsed_time/(double)Trials);
  }


  MPI_Finalize();

  return 0;
}
