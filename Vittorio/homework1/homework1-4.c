#include <stdio.h>          //printf()
#include <stdlib.h>         //malloc()
#include <string.h>         //strlen()
#include <time.h>
#include "mpi.h"            //MPI
#include "mycollection.h"   //broadcasting(),

#define ARRAY_SIZE 20
#define SOURCE 2

void testBroadcasting(){
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int *array = (int*) malloc(sizeof(int) * ARRAY_SIZE);
  if(world_rank == SOURCE){
    for(int i=0; i<ARRAY_SIZE;i++){
      array[i] = rand()%50;
    }
    broadcasting(array, ARRAY_SIZE, SOURCE, MPI_INT, MPI_COMM_WORLD);
  }
  else{
    broadcasting(array, ARRAY_SIZE, SOURCE, MPI_INT, MPI_COMM_WORLD);
    for(int i=0; i<ARRAY_SIZE;i++){
      printf("\nRank %d, indice %d, Valore %d",world_rank, i, array[i]);
    }
  }
}

void testScatter(){
  int x;
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int *array = (int*) malloc(sizeof(int) * ARRAY_SIZE);
  if(world_rank == SOURCE){
    for(int i=0; i<ARRAY_SIZE;i++){
      array[i] = rand()%50;
      //printf("Indice %d, ARRAY %d\n",i,array[i]);
    }
    scatter(array, ARRAY_SIZE, SOURCE, MPI_INT, MPI_COMM_WORLD, NULL);
  }
  else{
    scatter(array, ARRAY_SIZE, SOURCE, MPI_INT, MPI_COMM_WORLD, &x);
    for(int i=0; i<x;i++){
      printf("\nRank %d, indice %d, Valore %d",world_rank, i, array[i]);
    }
  }
}

void testGathering(){
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int tot_size;
  MPI_Comm_size(MPI_COMM_WORLD, &tot_size);
  int data;
  int *data_source;
  if(world_rank != SOURCE){
    gathering(&world_rank, tot_size,SOURCE, MPI_INT, MPI_COMM_WORLD, NULL);
  }
  else{
    data_source = (int*)malloc(sizeof(int) * tot_size-1);
    gathering(&data, tot_size,SOURCE, MPI_INT, MPI_COMM_WORLD, data_source);

    for(int i=0;i<tot_size-1;i++)
    printf("%d\n", data_source[i]);
  }
}

int main(int argc, char ** args){
  MPI_Init(&argc, &args);
  MPI_Status *status;
  MPI_Barrier(MPI_COMM_WORLD);
  double start = MPI_Wtime();

  //testBroadcasting();
  testGathering();
  //testScatter();

  MPI_Barrier(MPI_COMM_WORLD);
  double end = MPI_Wtime();
  printf("\nTime in ms = %f\n",end-start);

  MPI_Finalize();
}
