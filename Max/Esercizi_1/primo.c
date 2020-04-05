//scambio di valori interi fra 2 processi mpi

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status Stat;

    if(world_rank==0){
        int number0= 10;
        int number1receive;
        MPI_Send(&number0, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&number1receive, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &Stat);
        printf("processo 0: numero ricevuto %d\n", number1receive);
    }
    else if(world_rank==1){
        int number1=12;
        int number0receive;
        MPI_Recv(&number0receive, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &Stat);
        MPI_Send(&number1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("processo 1: numero ricevuto %d\n", number0receive);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}