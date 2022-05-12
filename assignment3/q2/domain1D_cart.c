#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;

    int dim[1], period[1], reorder, cart_rank, cart_size, coord[1];

    // number of cells (global)
    int nxc = 1024; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain


    int err = MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    if (err != MPI_SUCCESS){
      printf("Failed to initialize MPI process");
      return -1;
    }

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    dim[0] = size; //processors in each direction
    period[0] = 1; //periodic(=ture) or not(=false)
    reorder = 1;   //allow to reorder(=1)
    MPI_Comm comm_cart;           //comm_cart is communicator with new cartesian topology (handle)
    MPI_Cart_create(MPI_COMM_WORLD, 1, dim, period, reorder, &comm_cart);

    // Get the rank and size of the process
    MPI_Comm_rank(comm_cart, &cart_rank);
    MPI_Comm_size(comm_cart, &cart_size);
    // get coordinate of this rank
    MPI_Cart_coords(comm_cart, cart_rank, cart_size, coord);
    //printf("%d,%d\n",cart_rank, coord[0]);

    // get relationship between each rank
    int x_plus_source[1], x_plus_dest[1];
    MPI_Cart_shift(comm_cart, 0, 1, x_plus_source, x_plus_dest);
    //printf("%d,%d,%d\n",x_plus_source[0], cart_rank,x_plus_dest[0]);

    // now we do something related to our case


    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells
    int nxn_loc = nxc/size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L/((double) cart_size);
    double dx = L / ((double) nxc);

    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *check = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++){
      f[i] = sin(L_loc*cart_rank + (i-1) * dx);
      check[i] = cos(L_loc*cart_rank + (i-1) * dx);
    }


    // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
    // communicate ghost cells

    //MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
    //             int dest, int sendtag,
    //             void *recvbuf, int recvcount, MPI_Datatype recvtype,
    //             int source, int recvtag, MPI_Comm comm, MPI_Status * status)

    // first send the right ghost point
    MPI_Sendrecv(&f[nxn_loc-3], 1, MPI_DOUBLE, x_plus_dest[0],   1,
                 &f[0],         1, MPI_DOUBLE, x_plus_source[0], 1, comm_cart, MPI_STATUS_IGNORE);
   // second send the left ghost point
    MPI_Sendrecv(&f[2],         1, MPI_DOUBLE, x_plus_source[0],  2,
                 &f[nxn_loc-1], 1, MPI_DOUBLE, x_plus_dest[0],    2, comm_cart, MPI_STATUS_IGNORE);


    // here we finish the calculations

    // calculate first order derivative using central difference
    // here we need to correct value of the ghost cells!
    for (i=1; i<(nxn_loc-1); i++)
      dfdx[i] = (f[i+1] - f[i-1])/(2*dx);


    // Print f values and also check if all value error is small
    if (cart_rank==0){ // print only rank 0 for convenience
        printf("My rank %d of %d\n", cart_rank, cart_size );
        printf("Here are my values for f including ghost cells\n");
        for (i=0; i<nxn_loc; i++){
          if (abs(dfdx[i] - check[i])/dx/dx > 10){
            printf("Error is too large\n");
            return -1;
          }
          printf("%f,%f,%f\n", dfdx[i], check[i], dx*dx);

        }
        printf("Successful!\n");
        printf("\n");
    }

    // check all the values




    MPI_Finalize();
}
