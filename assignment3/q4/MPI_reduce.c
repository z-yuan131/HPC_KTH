#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#define SEED     921
#define NUM_ITER 1000000000
int main(int argc, char* argv[])
{
    int count = 0, rank, size, provided;;
    double x, y, z, pi;
    int local_count = 0;

    int err = MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    if (err != MPI_SUCCESS){
      printf("Failed to initialize MPI process");
      return -1;
    }

    double start_time, stop_time, elapsed_time;
    start_time = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + SEED + rank*100); // Important: Multiply SEED by "rank" when you introduce MPI!

    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < NUM_ITER/size; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));

        // Check if point is in unit circle
        if (z <= 1.0)
        {
            local_count++;
        }
    }





    // reduction for non-blocking mpi process
    /*
    int MPI_Reduce(const void* send_buffer,
               void* receive_buffer,
               int count,
               MPI_Datatype datatype,
               MPI_Op operation,
               int root,
               MPI_Comm communicator);*/


    MPI_Reduce(&local_count, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);



    stop_time = MPI_Wtime();
    elapsed_time = stop_time - start_time;


    if (rank == 0){
      // Estimate Pi and display the result
      pi = ((double)count / (double)NUM_ITER)  * 4.0;

      printf("The result is %f\n", pi);
      printf("Execution time is %f\n", elapsed_time);

    }

    MPI_Finalize();
    return 0;

}
