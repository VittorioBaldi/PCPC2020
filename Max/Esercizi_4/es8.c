// Sviluppare un programma MPI che data una matrice di dimensione NxM e P processi,
// calcola il massimo per ogni riga e il minimo per ogni colonna della matrice utilizzando in modo equo i P processi.
// Alla terminazione il master scrive su standard output i risultati.

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define M 100
#define MYBIGNUMBER 100000;

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    
    // Get the number of processes and the rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int N = world_size;
    int A[N][M];
    int i, j;
    int irecvmax[world_size];
    int irecvmin[M];

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            A[i][j]= i + j;
        }
    }

    int max = 0;
    for (j = 0; j < M; j++)
    {   
        if(A[world_rank][j] > max)
        {
            max = A[world_rank][j];
        }
    }

    MPI_Gather(&max, 1, MPI_INT, irecvmax, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //int min = MYBIGNUMBER;
    //TODO: calcolare il minimo sulle colonne 
    
    if(world_rank == 0)
    {
        for (i = 0; i < world_size; i++)
        {
            printf("riga: %d\tmax: %d\n", i, irecvmax[i]);
        }
    }

    MPI_Finalize();
    return 0;
}