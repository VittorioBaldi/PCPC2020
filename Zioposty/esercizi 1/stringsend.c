#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 100


int main(int argc, char** argv){
    
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    MPI_Status *Status;

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int size;
    if(rank == 0){
        char localBuffer[MAXSIZE];
        printf("Inserire messaggio da inviare: \n");
        scanf("%s", localBuffer);
        size = strlen(localBuffer)+1;
        MPI_Send(&size, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        MPI_Send(localBuffer, size, MPI_CHAR, 1, 1, MPI_COMM_WORLD);

        printf("Proc0 | Inviato al Proc1: %s\n", localBuffer);
    }
    else if(rank == 1){
        char*  buffer;
        MPI_Recv(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, Status);
        printf("Proc1 | Ricevuta la size: %d\n", size-1);
        buffer = (char*) malloc(size);
        MPI_Recv(buffer, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, Status);
        printf("Proc1 | Ricevuto messaggio: %s\n", buffer);
        
    }   


    MPI_Finalize();
    return 0;
}