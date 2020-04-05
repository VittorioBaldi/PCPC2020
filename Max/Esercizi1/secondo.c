//scambiare una stringa tra due processi

#include<stdio.h>
#include<mpi.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char *argv[]){

    MPI_Init(NULL, NULL);

    // Get the number of the process
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Status status;

    if(world_rank==0){
        char word0[20]= "PIPPO";
        int word0size= strlen(word0) +1;
        int word1size;
        char* word1rec;

        MPI_Send(&word0size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(word0, word0size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&word1size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        word1rec= (char *) malloc(word1size * sizeof(char));
        
        MPI_Recv(word1rec, word1size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, &status);
        printf("processo0: string ricevuta  %s\n", word1rec);
    }
    else if(world_rank==1){
        char word1[20]= "PLUTO";
        int word1size= strlen(word1) +1;
        int word0size;
        char* word0rec;

        MPI_Recv(&word0size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        word0rec= (char *) malloc(word0size * sizeof(char));
        
        MPI_Recv(word0rec, word0size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("processo1: string ricevuta  %s\n", word0rec);

        MPI_Send(&word1size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(word1, word1size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}
