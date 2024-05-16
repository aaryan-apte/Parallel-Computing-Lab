#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int local_sum = 0, global_sum = 0, rank, size;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = atoi(argv[1]);
    int chunk_size = n / size;
    int start = rank * chunk_size + 1;
    int end = 0;

    if (rank == size - 1)
        end = n;
    else
        end = start + chunk_size - 1;

    for (int i = start; i <= end; ++i)
    {
        local_sum += i;
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
        printf("The sum is: %d\n\n", global_sum);

    return 0;
}