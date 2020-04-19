// Sviluppare un programma MPI che utilizza P processori,
// dove il processo con rank 0 in MPI_COMM_WORLD colleziona un array di double da tutti gli altri processi in MPI_COMM_WORLD.

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

    int recv_count = ARRAY_SIZE * world_size;
    double isend[ARRAY_SIZE];
    double irecv[recv_count];
    int i;
    
    
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        isend[i] = i * 2;
    }
    

    MPI_Gather(isend, ARRAY_SIZE, MPI_DOUBLE, irecv, ARRAY_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if(world_rank == 0)
    {
        for (int i = 0; i < recv_count; i++)
        {
            printf("processo: %d\tnum: %.1f\n", world_rank, irecv[i]);
        }
    }
    
    
    MPI_Finalize();
    return 0;
}