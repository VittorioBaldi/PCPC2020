#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include "mycollection.h"


void broadcasting(void* data, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator){
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank == source){
    for(int i=0; i<tot_size; i++){
        if(i!=world_rank){
        MPI_Send(data, count, datatype, i, 0, comunicator);
        }
    }
  }else{
    MPI_Recv(data, count, datatype, source, 0, comunicator, MPI_STATUS_IGNORE);
  }
}


void scatter(int* data, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator, int *y){
  char name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Type_get_name(datatype, name, &name_len);

  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int x;
  if(world_rank == source){
    int numero_processori = tot_size-1;
    int jobs = count;
    int lastStart=0;
    for(int i=0; i<tot_size;i++){
      if(i!=source){
        x = (int) jobs / numero_processori;
        MPI_Send(&x, 1, MPI_INT, i, 0, comunicator);
        MPI_Send(data+lastStart, x, datatype, i, 1, comunicator);
        jobs = jobs - x;
        numero_processori = numero_processori - 1;
        lastStart = lastStart + x;
      }
    }
  }
  else{
    MPI_Recv(y, 1, MPI_INT, source, 0, comunicator, MPI_STATUS_IGNORE);
    MPI_Recv(data, *y, datatype, source, 1, comunicator, MPI_STATUS_IGNORE);
  }
}


// Gathering, il processo con rank 0 riceve da tutti i processi 1,...,P-1 un valore intero
void gathering(int* data, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator, int* data_source){
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank != source){
    MPI_Send(data, 1, datatype, source, 1, comunicator);
  }
  else{
    int j=0;
    for(int i=0;i<tot_size;i++){
      if(i!=source){
          MPI_Recv(data, 1, MPI_INT, i, 1, comunicator, MPI_STATUS_IGNORE);
          data_source[j] = *data;
          j++;
      }
    }

  }
}
