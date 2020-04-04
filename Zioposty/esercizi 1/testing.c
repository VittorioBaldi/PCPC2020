#include <stdio.h>
#include <mpi.h>
#include "mycollective.h"

int main(int argc, char** argv){ 
    double start, end;
    MPI_Init(&argc,&argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Barrier(MPI_COMM_WORLD);
    /* tutti i processi sono inizializzati */
    start =MPI_Wtime();
    /*----------------*/

    scattering(rank, world_size);

    /*----------------*/
    MPI_Barrier(MPI_COMM_WORLD);/* tutti i processi hanno terminato */
    end =MPI_Wtime();
    MPI_Finalize();
    if(rank ==0){
        /* Master node scrive su stdout il tempo o su file */
    printf("Time in ms = %f\n", end-start);
    }
}