#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

void MyBroadcasting(int world_size, int world_ranks, int *numbers, int size);
void MyGathering(int world_size, int world_ranks, int number);
void MyScatter(int world_size, int world_ranks, int *numbers, int size);