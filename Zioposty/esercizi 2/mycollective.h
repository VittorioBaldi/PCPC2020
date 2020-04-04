#define N 35

void broadcast(int rank, int world_size, int *buffer, int size);
void gathering(int rank, int world_size);
void scattering(int rank, int world_size);
