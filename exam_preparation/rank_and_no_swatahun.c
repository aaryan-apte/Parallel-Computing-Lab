#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Hello World!!! from process %d\n", rank);
    MPI_Finalize();

    if (rank == 0)
        printf("Number of processes: %d\n", size);

    return 0;
}
