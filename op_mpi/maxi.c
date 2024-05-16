#include <stdio.h>
#include <limits.h>
#include <omp.h>
#include <mpi.h>

int ARRAY_SIZE = 1000;

int main(int argc, char *argv[])
{
    int max_number = INT_MIN, rank, size;
    int array[ARRAY_SIZE];

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
    }
    printf("\n");

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_max = INT_MIN;

#pragma omp parallel for reduction(max : local_max)
    for (int i = rank; i < ARRAY_SIZE; i += size)
    {
        if (array[i] > local_max)
#pragma omp critical
        {
            local_max = array[i];
        }
    }

    MPI_Reduce(&local_max, &max_number, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
        printf("The maximum number is: %d\n", max_number);

    return 0;
}
