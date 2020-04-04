#include "mpi.h"
#include <stdio.h>


int main(int argc, char** argv){
   //Initialize MPI Enviroment
    MPI_Init(NULL, NULL);   //di norma dovrei passare i parametri del main
    
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status *status;

    int receive;
    int r;
    srand(time(NULL)); 
    if( world_rank == 0 ){
          // Initialization, should only be called once.
        r = rand() %20;      // Returns a pseudo-random integer between 0 and RAND_MAX.
    
        MPI_Send(&r, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(&receive, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, status);
        printf("Proc0: send %d, receive %d\n", r, receive);
    }
    else if( world_rank == 1) {
           // Initialization, should only be called once.
        r = rand() %8;      // Returns a pseudo-random integer between 0 and RAND_MAX.
    
        MPI_Recv(&receive, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, status);
        MPI_Send(&r, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("Proc1: send %d, receive %d\n", r, receive);
    }

    // Finalize the MPI environment.
    MPI_Finalize();

    return 0;
}