#include <stdio.h>          //printf()
#include <stdlib.h>         //malloc()
#include <string.h>         //strlen()
#include <time.h>
#include "mpi.h"            //MPI

#define array_size 20

void testBroadcasting(){
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int *array_to_send = (int*) malloc( sizeof(int) * array_size);
  if(world_rank == 0){
    for(int i=0; i<array_size;i++){
      array_to_send[i] = i;
    }
    for(int i=1; i<tot_size;i++)
      MPI_Send(array_to_send, array_size, MPI_INT, i, 0, MPI_COMM_WORLD);
  }
  else{
    MPI_Recv(array_to_send, array_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i=0; i<array_size;i++)
      printf("Rank %d, Indice %d, valore %d\n",world_rank, i, array_to_send[i]);
  }
}

void testGathering(){
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int *array_to_create= (int*) malloc( sizeof(int) * tot_size-1);
  if(world_rank != 0){
    MPI_Send(&world_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  else{
    int x;
    for(int i=0;i<tot_size-1;i++){
      MPI_Recv(&x, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      array_to_create[i] = x;
    }
    for(int i=0;i<tot_size-1;i++){
      printf("\nIndice %d, valore %d",i,array_to_create[i]);
    }
  }
}

void testScatter(){
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int *array_to_create= (int*) malloc( sizeof(int) * array_size);
  int x;
  if(world_rank == 0){
    for(int i=0; i<array_size;i++){
      array_to_create[i]=i;
    }
    int numero_processori = tot_size-1;
    int jobs = array_size;
    int lastStart=0;
    for(int i=0; i<tot_size-1;i++){
      x = (int) jobs / numero_processori;
      MPI_Send(&x, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
      MPI_Send(array_to_create+lastStart, array_size, MPI_INT, i+1, 1, MPI_COMM_WORLD);
      jobs = jobs - x;
      numero_processori = numero_processori - 1;
      lastStart = lastStart + x;
    }
  }
  else{
    MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(array_to_create, array_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i=0; i<x; i++)
      printf("Rank %d, Indice %d, Valore %d\n",world_rank, i, array_to_create[i]);
  }



}




int main(int argc, char ** args){
  MPI_Init(&argc, &args);

  MPI_Status *status;

  MPI_Barrier(MPI_COMM_WORLD);
  double start = MPI_Wtime();

  //testBroadcasting();
  //testGathering();
  //testScatter();

  MPI_Barrier(MPI_COMM_WORLD);
  double end = MPI_Wtime();
  printf("Time in ms = %f\n",end-start);

  MPI_Finalize();
}
