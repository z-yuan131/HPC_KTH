#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MSIZE 1024    //1024, a num which is pow of 2

#define SEED     921

#define Trials 10

double A[MSIZE][MSIZE];
double B[MSIZE][MSIZE];
double C[MSIZE][MSIZE];

// a function to allocate 2d arrays
double **alloc_2d_array(int rows, int cols) {
  int i;
  // first make a one dimentional array
  double *data = (double *)malloc(rows*cols*sizeof(double));
  // allocate each row to a new pointer
  double **array= (double **)malloc(rows*sizeof(double*));
  for (i=0; i<rows; i++)
    array[i] = &(data[cols*i]);

  return array;
}

double** Generate(int rows, int cols) {
    double** array = (double**) malloc(sizeof(double[cols]) * rows);

    for(int i = 0; i < rows; i++) {
        array[i] = malloc(sizeof(double) * cols);
        //for(int j = 0; j < cols; j++) {
        //    array[i][j] = 1;
        //}
    }

    return array;
}


void initialize_matrices(int Nsize, double **A_local, double **B_local, double **C_local, double **temp, int * coord)
{
  int i, j;
  srand(SEED);

  for (i = 0 ; i < MSIZE ; i++) {
    for (j = 0 ; j < MSIZE ; j++) {
      A[i][j] = (double) rand() / RAND_MAX;
      B[i][j] = (double) rand() / RAND_MAX;
      C[i][j] = 0.0;
      //printf("%f,%f\n", A[i][j], B[i][j]);
    }
  }

  for (i = 0; i < Nsize; i++){
    for (j = 0; j < Nsize; j++){
      A_local[i][j] = A[i + coord[0]*Nsize][j + coord[1]*Nsize];
      B_local[i][j] = B[i + coord[0]*Nsize][j + coord[1]*Nsize];
      C_local[i][j] = 0.0;
      temp[i][j] = 0.0;
    }
  }
}

void boardcast_matrices(int Nsize, int * dim, MPI_Comm row_comm,MPI_Comm comm_cart, int rank, int row_rank, int i, int y_source, int y_dest, double **A_local, double **B_local, double **A_buff, double **B_buff){
  //printf("%d,%d,%d\n", Nsize,dim[0],dim[1]);

  int root_rank;

  // initialize A_buff with local varibles
  for (int j = 0; j < Nsize; j++){
    for (int k = 0; k < Nsize; k++){
      A_buff[j][k] = A_local[j][k];
      if (i == 0){
        B_buff[j][k] = B_local[j][k];
      }
      else{
        B_local[j][k] = B_buff[j][k];     //update B_local
      }
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
  MPI_Bcast(&(A_buff[0][0]), (int)(Nsize*Nsize), MPI_DOUBLE, root_rank, row_comm);


  // get B from y direction neighbours
  //MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
  //             int dest, int sendtag,
  //             void *recvbuf, int recvcount, MPI_Datatype recvtype,
  //             int source, int recvtag, MPI_Comm comm, MPI_Status * status)

  // at the first iteration, this opration is not needed.
  if (i != 0){
    MPI_Sendrecv(&(B_local[0][0]), (int)(Nsize*Nsize), MPI_DOUBLE, y_source,   1,
                 &(B_buff[0][0]),  (int)(Nsize*Nsize), MPI_DOUBLE, y_dest,     1, comm_cart, MPI_STATUS_IGNORE);
  }
}


void calculate_C(int Nsize, double **C_local, double **A_buff, double **B_buff)
{
    for (int i = 0; i < Nsize; i++){
      for (int j = 0; j < Nsize; j++) {
        for (int k = 0 ; k < Nsize ; k++) {
          C_local[i][j] += A_buff[i][k]*B_buff[k][j];
        }
      }
    }
}


void average_result(int Nsize, int rank, int size, double **C_local, double **temp){
  //int MPI_Reduce(const void* send_buffer,
  //             void* receive_buffer,
  //             int count,
  //             MPI_Datatype datatype,
  //             MPI_Op operation,
  //             int root,
  //             MPI_Comm communicator);
  MPI_Reduce(&(C_local[0][0]), &(temp[0][0]), (int)(Nsize*Nsize), MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  int Rsize = (int)(Nsize * sqrt(size));
  double avg = 0.0;
  if (rank == 0){
    for (int i = 0; i < Nsize; i++){
      for (int j = 0; j < Nsize; j++){
        avg += temp[i][j];
      }
    }

    //printf("parallel summation result is %f\n", avg/(double)(Rsize*Rsize)/Trials);
  }
}

void average_result_for_serial_code(int Nsize, int size){
  int Rsize = (int)(Nsize * sqrt(size));
  //printf("%d\n",Rsize);

  double avg = 0.0;

  for (int trial = 0; trial < Trials; trial++){
    for (int i = 0 ; i < Rsize ; i++) {
      for (int j = 0 ; j < Rsize ; j++) {
        for (int k = 0 ; k < Rsize ; k++) {
          avg += A[i][k] * B[k][j];
        }
      }
    }
  }


  //printf("serial summation result is %f \n", avg/(double)(Rsize*Rsize)/Trials);

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

  // define A_local,B_local,C_local,A_buff,B_buff and reduction matrix temp

  double **A_local = Generate(Nsize,Nsize);
  double **B_local = Generate(Nsize,Nsize);
  double **C_local = Generate(Nsize,Nsize);
  double **A_buff = Generate(Nsize,Nsize);
  double **B_buff = Generate(Nsize,Nsize);
  double **temp = Generate(Nsize,Nsize);



  //initialize_matrices(Nsize, A_local, B_local, C_local, temp, coord);



  // main solver
  for (int trial = 0; trial < Trials; trial++){
    initialize_matrices(Nsize, A_local, B_local, C_local, temp, coord);
    for (int i = 0; i < dim[1]; i++){
      boardcast_matrices(Nsize,dim,row_comm,comm_cart,rank,row_rank,i,y_source[0],y_dest[0],A_local,B_local,A_buff,B_buff);

      calculate_C(Nsize, C_local, A_buff, B_buff);
    }

  }



  MPI_Comm_free(&row_comm);
  MPI_Comm_free(&comm_cart);


  stop_time = MPI_Wtime();
  elapsed_time = stop_time - start_time;

  average_result(Nsize, rank, size, C_local, temp);



  if (rank == 0)
  {
    start_time = MPI_Wtime();
    average_result_for_serial_code(Nsize, size);
    stop_time = MPI_Wtime();

    printf("Final elapsed time for parallel job on %d ranks is %f \n", size, elapsed_time/(double)Trials);
    //printf("Final elapsed time for serial job is %f \n", (stop_time - start_time)/(double)Trials);
  }


  MPI_Finalize();

  return 0;
}
