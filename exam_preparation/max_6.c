#include <stdio.h>
#include <mpi.h>

#define ARRAY_SIZE 6

int main(int argc, char **argv)
{
    int rank, size;
    int local_array[ARRAY_SIZE], global_max, local_max;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Generate or initialize local_array
    if (rank == 0)
    {
        printf("Enter %d integers:\n", ARRAY_SIZE);
        for (int i = 0; i < ARRAY_SIZE; i++)
        {
            scanf("%d", &local_array[i]);
        }
    }

    // Scatter the array to all processes
    MPI_Scatter(local_array, ARRAY_SIZE, MPI_INT, local_array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank; i < 6; i += size)
    {
        if (local_array[i] > local_max)
            local_max = local_array[i];
    }

    // // Find local max
    // local_max = local_array[0];
    // for (int i = 1; i < ARRAY_SIZE / size; ++i)
    // {
    //     if (local_array[i] > local_max)
    //     {
    //         local_max = local_array[i];
    //     }
    // }

    // Find global max using MPI reduction
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // Print global max by rank 0
    if (rank == 0)
    {
        printf("The maximum value in the array is: %d\n", global_max);
    }

    MPI_Finalize();
    return 0;
}
