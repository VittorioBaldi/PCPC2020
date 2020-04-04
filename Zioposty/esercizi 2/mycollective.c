#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include "mycollective.h"


void fillArray(int buffer[]){

    int i;
    for( i = 0; i< N; i++)
        buffer[i] = i;
}


void broadcast(int rank, int world_size, int *buffer, int size){
    
    
    if(rank == 0){
       // fillArray(buffer);
        printf("Array ready\nStarting Broadcast\n");
        int i;
        for(i = 1; i<world_size; i++)
            MPI_Send(buffer, size, MPI_INT, i, 1, MPI_COMM_WORLD);
            
        printf("Proc0: broadcast executed\n");
    }
    else
    {
        MPI_Status *Status;
        MPI_Recv(buffer, N, MPI_INT, 0, 1, MPI_COMM_WORLD, Status);
        
        int i = 0;
        printf("Proc%d: ", rank);
        for(; i < N; i++)
            printf("%d ", buffer[i]);
        
        printf("\n");

    }
    return;
}

void gathering(int rank, int world_size){

    if(rank == 0){
        MPI_Status *Status;
        int *buffer = (int*) malloc(sizeof(int)*(world_size -1));
        int i;
        
        for(i = 0; i< world_size-1; i++){
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, i, 1, MPI_COMM_WORLD, Status);
            buffer[i] = temp;
        }

        printf("Proc0 | Gathering Completed: ");

        for(i = 1; i< world_size; i++)
            printf("%d ", buffer[i-1]);
        
        printf("\n");
        free(buffer);
    }
    else{
        MPI_Send(&rank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        printf("Proc%d | sended: %d\n", rank, rank);
    }  

}

void scattering(int rank, int world_size){

    if(rank == 0){
        int buffer[N];
        fillArray(buffer);
        
        int i;
        int n = N;
        int p = world_size-1;
        int size;
        int *temp = buffer;
        for(i = 0; i< world_size-1; i++){
            size = (int) n/p;
            //temp = (int*) malloc(sizeof(int)*size);
            /*
            int j = 0;
            for(; j<size; j++) 
                temp[j] = buffer[start+j];
            */
            
            MPI_Send(&size, 1, MPI_INT, i+1, 1, MPI_COMM_WORLD);
            MPI_Send(temp, size, MPI_INT, i+1, 2, MPI_COMM_WORLD);

            n = n - size;
            p--;
            temp  = temp + size;
        }
        printf("Proc0 | Scattering ultimated\n");
    }
    else{
        int *part;
        int size;
        MPI_Status *Status;
        MPI_Recv(&size, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, Status);
        part = (int*) malloc(size * sizeof(int));
        MPI_Recv(part, size, MPI_INT, 0, 2, MPI_COMM_WORLD, Status);

        int i = 0;
        printf("Proc%d | ", rank);

        for(; i<size; i++)
            printf("%d ", part[i]);
        printf("\n");

        free(part);
    }   

}