//Invio di una stringa (letta da stdin), da parte del processo con rank 0,, al processo con rank1

#include <mpi.h>    //MPI
#include <stdio.h>  //printf()
#include <stdlib.h> //malloc()
#include <string.h> //strlen()

int main(int argc, char ** args){
  int tot_size, world_rank, name_len, size, src=0, dest=1;        
  char processor_name[MPI_MAX_PROCESSOR_NAME], *string;        
                         
  MPI_Status status;

  MPI_Init(&argc, &args);
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Get_processor_name(processor_name, &name_len);

  if(world_rank == 0){
    printf("Size of array?\n");
    scanf("%d",&size);
    MPI_Send(&size, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    string = malloc(sizeof(char*) * size);
    printf("Ciao! Sono il processore %s con rank %d, inserisci una stringa da inviare al rank1\n",processor_name, world_rank);
    scanf("%s",string);
    MPI_Send(string, strlen(string)+1, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
  }
  else if(world_rank == 1){
    MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    string = malloc(sizeof(char*) * size);
    MPI_Recv(string, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
    printf("Ciao! Sono il processore %s con rank %d\n",processor_name, world_rank);
    printf("La stringa ricevuta dal rank0 e' :%s\n",string);
  }
  // Finalize the MPI environment.
  MPI_Finalize();
}
