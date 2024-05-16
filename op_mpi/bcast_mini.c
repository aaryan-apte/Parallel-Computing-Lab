#include <stdio.h>
#include <limits.h>
#include <omp.h>
#include <mpi.h>

int ARRAY_SIZE = 1000;

int main(int argc, char *argv[])
{
    int min_number = INT_MAX, rank, size;
    int array[ARRAY_SIZE];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_min = INT_MAX;

    if (rank == 0)
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            array[i] = rand() % 100 + 1;
            printf("%d ", array[i]);
        }
        printf("\n");
    }
    MPI_Bcast(array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

#pragma omp parallel for reduction(min : local_min)
    for (int i = rank; i < ARRAY_SIZE; i += size)
    {
        if (array[i] < local_min)
        {
            local_min = array[i];
        }
    }

    MPI_Reduce(&local_min, &min_number, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
        printf("The minimum number is: %d\n", min_number);

    return 0;
}
