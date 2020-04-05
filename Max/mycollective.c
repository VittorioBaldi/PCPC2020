#include"mycollective.h"

//static functions
static int calculate_max(int *numbers, int size, int world_size, int world_rank)
{
    if(world_rank == 0)
    {
        int piece= size / world_size; //indica quanto devo assegnare ad ogni processo
        //da 0 a piece-1 per il primo processo
        //da piece a size-1 per il resto dei processi incrementando sempre di piece

        //realizzo una scatter
        for (int i = 1; i < world_size; i++)
        {
            int proc_i_first_element = i * piece; 
            MPI_Send(&piece, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&proc_i_first_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        printf("AAAAA");
        //computo il massimo tra 0 e piece-1
        int max_proc[world_size];
        max_proc[0] = numbers[0];
        for (int i = 0; i < piece; i++)
        {
            if(numbers[i] > max_proc[0])
            {
                max_proc[0] = numbers[i];
            }
        }

        printf("BBBBBB");
        //ricevo il massimo calcolato dagli altri processi
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(&max_proc[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);
        }


        printf("CCCCCC");
        //calcolo il massimo tra tutti i massimi
        int max = max_proc[0];
        for (int i = 0; i < world_size; i++)
        {
            if(max_proc[i] > max)
            {
                max = max_proc[i];
            }
        }
        
        return max;
    }
    else
    {
        int piece;
        int numbers_start;
        int max = numbers[numbers_start];

        MPI_Recv(&piece, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&numbers_start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);

        for (int i = 0; i < piece; i++)
        {
            if(numbers[numbers_start + i] > max)
            {
                max = numbers[numbers_start + i];
            }
        }
        
        MPI_Send(&max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    return 0;
}

static int calculate_min(int *numbers, int size, int world_size, int world_rank)
{
    if(world_rank == 0)
    {
        int piece= size / world_size; //indica quanto devo assegnare ad ogni processo
        //da 0 a piece-1 per il primo processo
        //da piece a size-1 per il resto dei processi incrementando sempre di piece

        //realizzo una scatter
        for (int i = 1; i < world_size; i++)
        {
            int proc_i_first_element = i * piece; 
            MPI_Send(&piece, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&proc_i_first_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        //computo il minimo tra 0 e piece-1
        int min_proc[world_size];
        min_proc[0] = numbers[0];
        for (int i = 0; i < piece; i++)
        {
            if(numbers[i] < min_proc[0])
            {
                min_proc[0] = numbers[i];
            }
        }

        //ricevo il minimo calcolato dagli altri processi
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(&min_proc[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);
        }

        //calcolo il minimo tra tutti i massimi
        int min = min_proc[0];
        for (int i = 0; i < world_size; i++)
        {
            if(min_proc[i] < min)
            {
                min = min_proc[i];
            }
        }
        
        return min;
    }
    else
    {
        int piece;
        int numbers_start;
        int min = numbers[numbers_start];

        MPI_Recv(&piece, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&numbers_start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);

        for (int i = 0; i < piece; i++)
        {
            if(numbers[numbers_start + i] < min)
            {
                min = numbers[numbers_start + i];
            }
        }
        
        MPI_Send(&min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    return BIG_NUMBER;
}

static int calculate_avg(int *numbers, int size, int world_size, int world_rank)
{
    if(world_rank == 0)
    {
        int piece= size / world_size; //indica quanto devo assegnare ad ogni processo
        //da 0 a piece-1 per il primo processo
        //da piece a size-1 per il resto dei processi incrementando sempre di piece

        //realizzo una scatter
        for (int i = 1; i < world_size; i++)
        {
            int proc_i_first_element = i * piece; 
            MPI_Send(&piece, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&proc_i_first_element, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        //computo la somma tra 0 e piece-1
        int sum_proc[world_size];
        sum_proc[0] = 0;
        for (int i = 0; i < piece; i++)
        {
            sum_proc[0] += numbers[i];
        }

        //ricevo la somma calcolata dagli altri processi
        for (int i = 1; i < world_size; i++)
        {
            MPI_Recv(&sum_proc[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, NULL);
        }

        //calcolo la somma totale
        int sum=0;
        for (int i = 0; i < world_size; i++)
        {
            sum += sum_proc[i];
        }
        
        //ritorno la media
        return sum/size;
    }
    else
    {
        int piece;
        int numbers_start;
        int sum=0;

        MPI_Recv(&piece, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        MPI_Recv(&numbers_start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);

        for (int i = 0; i < piece; i++)
        {
            sum += numbers[numbers_start + i];
        }
        
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    return 0;
}


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

int Reduce(int *numbers, int size, int world_size, int world_rank, int tipo){
    if (tipo == 0)
    {
        int max = calculate_max(numbers, size, world_size, world_rank);
        return max;
    }
    else if (tipo == 1)
    {
        int min = calculate_min(numbers, size, world_size, world_rank);
        return min;
    }
    else if (tipo == 2)
    {
        int avg = calculate_min(numbers, size, world_size, world_rank);
        return avg;
    }
}

