//Scambio di un valore intero tra due processi MPI.

#include <mpi.h>
#include <stdio.h>

int main(int argc, char ** args){
  int tot_size,      //numero di processi
      world_rank,    //rank del processore
      name_len,      //lunghezza nome processo
      a=10,          //valore del processore1
      b=15;          //valore del processore2

  char processor_name[MPI_MAX_PROCESSOR_NAME];  //nome del processore
  MPI_Init(&argc, &args);

  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Get_processor_name(processor_name, &name_len);

  if(world_rank == 0){
    printf("Sono il processore %s, con rank %d e mando il mio valore %d al processore con rank 1\n",processor_name, world_rank, a);
    MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&b, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
    printf("Sono il processore %s con rank %d e ho ricevuto il vlaore %d dal processore con rank1\n", processor_name, world_rank, b);
  }
  else if(world_rank == 1){
    MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    printf("Sono il processore con rank %d e ho ricevuto il valore %d dal processore con rank0\n", world_rank, a);
    printf("Sono il processore %s con rank %d mando il valore %d al processore con rank0?\n", processor_name, world_rank, b);
    MPI_Send(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }
  // Finalize the MPI environment.
  MPI_Finalize();
}
