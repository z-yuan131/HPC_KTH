#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mpi.h>

//#define MAX_SIZE   134217728
#define NUM_ROUNDS 50

int main(int argc, char **argv) {
  int        me, nproc,soi;
  int        round;
  int        i,j,msg_length;
  double     t_start, t_stop;
  //u_int8_t  *snd_buf;  // Send buffer (byte array)
  //u_int8_t  *rcv_buf;  // Receive buffer (byte array)
  double *snd_buf, *rcv_buf;
  MPI_Win    window;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  if (nproc != 2) {
    if (me == 0) printf("This benchmark should be run on exactly two processes");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

/*  if (me == 0)
    printf("MPI-2 passive ping-pong latency test, performing %d rounds at each xfer size.\n\n", NUM_ROUNDS);
*/
for (j = 0; j <= 27; j++){
  long int N = 1 << j;
  //printf("N=%ld\n",N);
  MPI_Alloc_mem(N*sizeof(double), MPI_INFO_NULL, &rcv_buf);
  MPI_Alloc_mem(N*sizeof(double), MPI_INFO_NULL, &snd_buf);
  //double *rcv_buf=(double*)malloc(N*sizeof(double));
  //double *snd_buf=(double*)malloc(N*sizeof(double));
  
  //printf("mem clear");
  MPI_Win_create(rcv_buf, N*sizeof(double), sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &window);
//[initialize!]
  for (i = 0; i < N; i++) {
    snd_buf[i] = 1.0;
  }
  //printf("init clear");
  //for (msg_length = sizeof(double); msg_length <= N * sizeof(double); msg_length *= 2) {
    MPI_Barrier(MPI_COMM_WORLD);
    t_start = MPI_Wtime();

    // Perform NUM_ROUNDS ping-pongs
    for (round = 0; round < NUM_ROUNDS; round++) {

      // I am the sender
      if (round % 2 == me) {
        // Clear start and end markers for next round
        /*MPI_Win_lock(MPI_LOCK_EXCLUSIVE, me, 0, window);
        rcv_buf[0] = 0;
        rcv_buf[msg_length-1] = 0;
        MPI_Win_unlock(me, window);*/

        MPI_Win_lock(MPI_LOCK_EXCLUSIVE, (me+1)%2, 0, window);
        MPI_Put(snd_buf, N, MPI_DOUBLE, (me+1)%2, 0, N, MPI_DOUBLE, window);
        MPI_Win_unlock((me+1)%2, window);
      }

      // I am the receiver: Poll start and end markers
      /*else {
        u_int8_t val; 

        do {
          //MPI_Iprobe(0, 0, MPI_COMM_WORLD, &val, MPI_STATUS_IGNORE);
          MPI_Win_lock(MPI_LOCK_EXCLUSIVE, me, 0, window);
          val = ((volatile u_int8_t*)rcv_buf)[0];
          MPI_Win_unlock(me, window);
        } while (val == 0);
	
        do {
          //MPI_Iprobe(0, 0, MPI_COMM_WORLD, &val, MPI_STATUS_IGNORE);
          MPI_Win_lock(MPI_LOCK_EXCLUSIVE, me, 0, window);
          val = ((volatile u_int8_t*)rcv_buf)[msg_length-1];
          MPI_Win_unlock(me, window);
        } while (val == 0);
      } */
    }

    MPI_Barrier(MPI_COMM_WORLD);
    t_stop = MPI_Wtime();

    if (me == 0)
      printf("%10ld\t%15.9f\n", N*sizeof(double), (t_stop-t_start)/NUM_ROUNDS);
  //}

  MPI_Win_free(&window);
  MPI_Free_mem(snd_buf);
  MPI_Free_mem(rcv_buf);
}
  MPI_Finalize();
  return 0;
}
