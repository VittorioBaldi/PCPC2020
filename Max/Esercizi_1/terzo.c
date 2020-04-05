/*
Dati P processi MPI e un array di valori interi e di lunghezza N,
si eseguano le seguenti operazioni:
    -Broadcasting: il processo con rank 0 invia a tutti i processi 1...P-1;
    -Gathering: il processo con rank 0 riceve da tutti i processi 1... P-1 un valore intero;
    -Scatter: il processo con rank 0 invia una porzione di array ad ogni processo in 1...P-1.
*/


#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include"../mycollective.h"

int main(int argc, char const *argv[])
{
    MPI_Init(NULL, NULL);

    //get num process
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //get rank process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int numbers[]= {1,3,4,5,6,10,12,20,25,26,125,124,1,25,12,5,1,26,12,41,2,412,51,2,41,24,14,2};
    int size= sizeof(numbers) / sizeof(int);

    double start, end;
    MPI_Barrier(MPI_COMM_WORLD);
    start= MPI_Wtime();

    MyBroadcasting(world_size, world_rank, numbers, size);
    MyGathering(world_size, world_rank, numbers[world_rank]);
    MyScatter(world_size, world_rank, numbers, size);

    MPI_Barrier(MPI_COMM_WORLD);
    end= MPI_Wtime();

    MPI_Finalize();

    if(world_rank == 0){
        printf("Tempo in ms = %f\n", end-start);
    }
}