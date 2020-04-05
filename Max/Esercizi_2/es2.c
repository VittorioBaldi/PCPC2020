// Calcolare il massimo, il  minimo, la media di array di valori interi utilizzando P processi
// ed operazioni di comunicazione in modalità bloccante.

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include"../mycollective.h"

#define SIZE_ARRAY 100

int main(int argc, char **argv)
{
     // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int numbers[SIZE_ARRAY];
    int i;
    //inizializzo l'array con i primi SIZE_ARRAY numeri dispari
    for(i = 0; i < SIZE_ARRAY; i++)
    {
        numbers[i]= 2 * i + 1;
    }

    int max = Reduce(numbers, SIZE_ARRAY, world_size, world_rank, 0);
    int min = Reduce(numbers, SIZE_ARRAY, world_size, world_rank, 1);
    int avg = Reduce(numbers, SIZE_ARRAY, world_size, world_rank, 2);

    if(world_rank == 0)
    {
        printf("\nNUMERI\n");
        for (int i = 0; i < SIZE_ARRAY; i++)
        {
            printf("%d ", numbers[i]);
        }
        
        printf("\n\nMASSIMO %d\n\n", max);
        printf("\nMINIMO %d\n\n", min);
        printf("\nMEDIA %d\n\n", avg);
    }

    MPI_Finalize();
}

