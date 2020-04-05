#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define BIG_NUMBER 1000000

void MyBroadcasting(int world_size, int world_ranks, int *numbers, int size);
void MyGathering(int world_size, int world_ranks, int number);
void MyScatter(int world_size, int world_ranks, int *numbers, int size);

/**
 *  Realizza le funzioni di Max, Min, Avg di un array di interi
 * tipo = 0 Max
 * tipo = 1 Min
 * tipo = 2 Avg
 */
int Reduce(int *numbers, int size, int world_size, int world_rank, int tipo);