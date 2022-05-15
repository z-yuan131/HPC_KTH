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
    int count = 0, rank, size, provided, source,dest,tag;
    double x, y, z, pi;
    int local_count = 0, generation;

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

    // initialize count with local_count
    count += local_count;

    // binary tree reduction for blocking mpi process
    // generation is how many tree branches should be reducted
    generation = (int)(log(size) / log( 2 ));

    for (int gen = generation; gen > 0; gen--){

        if (rank < pow(2,gen-1)){
          source = (int)(rank + pow(2,gen-1));
          MPI_Recv(&local_count, 1, MPI_INT, rank + pow(2,gen-1), rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          count += local_count;
        }
        else if (rank >= pow(2,gen-1) && rank < pow(2,gen)){
          tag = (int)(rank - pow(2,gen-1));
          dest = (int)(rank - pow(2,gen-1));

          MPI_Send(&count, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }

    }

    stop_time = MPI_Wtime();
    elapsed_time = stop_time - start_time;


    if (rank == 0){
      // Estimate Pi and display the result
      pi = ((double)count / (double)NUM_ITER)  * 4.0;

      //printf("The result is %f \n", pi);
      printf("Execution time is %f \n", elapsed_time);

    }

    MPI_Finalize();
    return 0;

}
