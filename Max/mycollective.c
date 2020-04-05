#include"mycollective.h"

void MyBroadcasting(int world_size, int world_rank, int *numbers, int size){
    //status variable
    MPI_Status status;

    if(world_rank==0)
    {
        //BROADCASTING
        printf("Siamo nella fase di BROADCASTING\n");
        
        int i;
        for(i = 1; i < world_size; i++)
        {
            MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(numbers, size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        int* numbers;
        int size;
        
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        numbers= (int *) malloc(size * sizeof(int));
        
        MPI_Recv(numbers, size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("processo %d:", world_rank);
        int j;
        for(j = 0; j < size; j++)
        {
            printf(" %d", numbers[j]);
        }
        printf("\n");

        free(numbers);
    }
}

void MyGathering(int world_size, int world_ranks, int number){
    //status variable
    MPI_Status status;

    if(world_ranks==0){
        printf("SIAMO NELLA FASE DI GATHERING\n");
        int i;
        int buffer[world_size-1];
        for(i = 1; i < world_size; i++)
        {
            MPI_Recv(&buffer[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }

        printf("processo 0: numberi ricevuti");
        for(i = 0; i < world_size-1; i++)
        {
            printf(" %d", buffer[i]);
        }
        printf("\n");
    }
    else
    {
        MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
}

void MyScatter(int world_size, int world_rank, int *numbers, int size){
    //status variable
    MPI_Status status;
    int piece= size / (world_size-1);
    
    if(world_rank == 0)
    {
        printf("SIAMO DELLA FASE DI SCATTER\n");
        int i;
        for(i = 1; i < world_size-1; i++){
            int j= i-1;
            int start = j * piece;
            int end =  (j + 1) * piece - 1; 

            MPI_Send(&piece, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&numbers[start], piece, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        if(i == world_size-1)
        {
            int start = (i-1) * piece;
            int endpiece = size - start;
            MPI_Send(&endpiece, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&numbers[start], endpiece, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        int buffsize;
        int *buff;
        int i;

        MPI_Recv(&buffsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        buff = (int *) malloc(buffsize * (sizeof(int)));
        MPI_Recv(buff, buffsize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        printf("processo %d: numeri ricevuti", world_rank);
        for(i = 0; i < buffsize; i++){
            printf(" %d", buff[i]);
        }
        printf("\n");
        free(buff);
    }
    
}