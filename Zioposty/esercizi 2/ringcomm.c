#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "mpi.h"
#include "mycollective.h"
#define GAMES 10
#define S 300


int ringGame(int rank, int world_size);

int main(int argc, char** argv){
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    int  i, count =0;
    srand(rank);

    for(i = 0; i < GAMES; i++){
        if(rank == 0){
            printf("\n-----------\nProc0 | Game  %d\n", i+1);
            }

        count = count + ringGame(rank, world_size);
        MPI_Barrier(MPI_COMM_WORLD);
        
    }
        
     MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0)
        printf("\n--------------\nProc0 | iterazioni medie per superare la soglia: %d\n", count/GAMES);

    MPI_Finalize();

}


int ringGame(int rank, int world_size){

    //ogni proc inizializza il proprio seme
    int sum = 0;
    bool finished = false;
    MPI_Status *Status;
    int count = 0;

    if(rank == 0){
        
        while(!finished){
            count++;
            sum = sum + (rand()%100 + 1);
            if( sum > S)
            {
                finished  = true;
                continue;
            }
            
            MPI_Send(&sum, 1, MPI_INT, (rank+1)%world_size, (rank+1)%world_size, MPI_COMM_WORLD);
            printf("Proc0 | send %d to %d\n", sum, (rank+1)%world_size );
            fflush(stdout);
            MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, Status);
            
            //se sum > S qualcuno ha superato la soglia e lo ha notificato al Proc0
            finished = (sum>S);
        }

        //Invio di notifica di terminazione
        broadcast(rank, world_size, &sum, 1);
        printf("Proc0 | count = %d\n", count);
        fflush(stdout);
        return count;
    }
    else{

        while(!finished){
            MPI_Recv(&sum, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, Status);
            
            //Ho ricevuto un valore superiore alla soglia, devo terminare
            if(sum > S){
                finished = true;
                continue;
            }
            else sum =sum + rand()%100 + 1;
            
            //se calcolo un valore minore della soglia, passo il val al next proc, altrimenti notifico il proc0
            if( sum <= S)
            {
                MPI_Send(&sum, 1, MPI_INT, (rank+1)%world_size, (rank+1)%world_size, MPI_COMM_WORLD);
                printf("Proc%d | Sent %d to %d\n", rank, sum, (rank+1)%world_size);
                fflush(stdout);
            }
            else{
                MPI_Send(&sum, 1, MPI_INT, 0, (rank+1)%world_size, MPI_COMM_WORLD);
                printf("Proc%d | Notify 0  value=%d\n", rank, sum);
                fflush(stdout);
            }
        }

        return 0;     
    }

 }
