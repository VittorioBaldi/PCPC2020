#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include"mpi.h"
#include"mycollective.h"

int game(int soglia, int world_rank, int world_size);

int main(int argc, char **args)
{
    // Initialize the MPI environment
    MPI_Init(&argc, &args);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    srand(time(0));
    //1+rand()%100 numeri casuali tra 1 e 100

    int soglia= atoi(args[1]);
    int rounds[10];
    int i;
    for(i = 0; i < 10; i++)
    {
        if(world_rank == 0) printf("\nITERAZIONE %d\n", i);
        rounds[i]= game(soglia, world_rank, world_size);
    }

    if(world_rank==0)
    {
        float sum = 0;
        for(i = 0; i < 10; i++)
        {
            sum += rounds[i];
        }
        printf("sum: %f\n", sum);
        float media = sum/10;

        printf("media di round per raggiungere la soglia %f\n", media);
    }


    MPI_Finalize();
}

int game(int soglia, int world_rank, int world_size)
{
    int finito = 0;
    int n=0;
    int round=1;
    MPI_Status Stat;
    int num=0;

    while(1)
    {
        if(world_rank==0)
        {
            //1 + rand() % 100 va in loop
            n= n + 10;
            if(n >= soglia)
            {
                break;
            }
            int next= (world_rank + 1) % world_size;
            int recv;
            MPI_Send(&n, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Stat);
            if(recv >= soglia)
            {
                int i;
                for(i = 1; i < world_size; i++)
                {
                    MPI_Send(&recv, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                    printf("recv: %d, process: %d\n", recv, i);
                }
                break;
            }
            else
            {
                round++;
                n = recv;
            }
        }
        else
        {
            int n;
            int next = (world_rank + 1) % world_size;
            int prev = (world_rank - 1) % world_size;
            MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Stat);
            if(n >= soglia)
            {
                break;
            }

            n = n + 10;
            if(n >= soglia)
            {
                MPI_Send(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Send(&n, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            }
        }
    }

    return round;
}