// Sviluppare un programma MPI che utilizza P processori,
// dove il processo con rank 0 in MPI_COMM_WORLD invia un array di double a tutti gli altri processi in MPI_COMM_WORLD.

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define ARRAY_SIZE 10

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    
    // Get the number of processes and the rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    double buffer[ARRAY_SIZE];
    int i;
    
    if(world_rank == 0)
    {
        for (i = 0; i < ARRAY_SIZE; i++)
        {
            buffer[i] = i * 2;
        }
    }

    MPI_Bcast(buffer, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("processo: %d\tnum: %.1f\n", world_rank, buffer[i]);
    }
    
    MPI_Finalize();
    return 0;
}
