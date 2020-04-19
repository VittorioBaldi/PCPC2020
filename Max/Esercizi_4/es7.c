// Sviluppare un programma MPI che dato un array, A di interi di lunghezza N,
// utilizza equamente P processori per aggiornare i valori di A. Ogni elemento A[i] è calcolato utilizzando la seguente operazione:
//  A[i] = A[i-1] + A[i] + A[i+1], i = 1 ... N-2
//  A[0] = A[N-1] + A[0] + A[1], i=0
//  A[N-1] = A[N-2] + A[N-1] + A[0], i = N-1
// l'array A viene inizializzato nel processo master e gli slave eseguono le operazioni solo sulla propria porzione di array;
// ogni processo slave invia la sua porzione di array nuovamente al master;
// alla terminazione delle recezioni il processo master scrive su standard output il tempo di esecuzione.

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define ARRAY_SIZE 1000

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    
    // Get the number of processes and the rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int array[ARRAY_SIZE];
    int i;

    for (i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = i * 3;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    int time = MPI_Wtime();

    //indica la porzione di array assegnata ad ogni processo
    int piece = ARRAY_SIZE / world_size;
    //array di appoggio per inviare i nuovi valori
    int isend[piece];

    
    int init = world_rank * piece; //indice iniziale array per ogni processo
    int j;

    for (j = 0; j < piece; j++)
    {
        int prec;
        if(init + j != 0)   //caso speciale per il primo indice
        {
            prec = (init + j - 1) % ARRAY_SIZE; //indice precedente
        }
        else
        {
            prec = ARRAY_SIZE - 1;
        }
        
        
        int att = init + j; //indice attuale
        int succ = (init + j + 1) % ARRAY_SIZE;  //indice successivo
        isend[j] = array[prec] + array[att] + array[succ];
    }
    
    
    MPI_Gather(isend, piece, MPI_INT, array, piece, MPI_INT, 0, MPI_COMM_WORLD);

    if(world_rank == 0)
    {
        printf("processore: %d\ttempo impiegato: %1.2f\n", world_rank, MPI_Wtime()-time);
    }
    
    MPI_Finalize();
    return 0;
}
