void broadcasting(void* data, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator);

void scatter(int* datavoid, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator, int *x);

void gathering(int* data, int count, int source, MPI_Datatype datatype, MPI_Comm comunicator, int* data_source);
