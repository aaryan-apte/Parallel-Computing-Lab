#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int rank, size, n;
    long long int local_sum = 0, global_sum = 0;

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define the range, for example, we can take n from the command line
    if (argc != 2)
    {
        if (rank == 0)
        {
            fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    double start_time = MPI_Wtime();
    n = atoi(argv[1]);

    printf("The rank: %d and size: %d\n", rank, size);

    // Calculate the local range for each process
    int chunk_size = n / size;
    int start = rank * chunk_size + 1;

    int end = (rank == size - 1) ? n : start + chunk_size - 1;

    // Compute the local sum
    for (int i = start; i <= end; ++i)
    {
        local_sum += i;
    }

    // Reduce all local sums to a global sum
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // The root process (rank 0) prints the global sum
    if (rank == 0)
    {
        printf("Sum of numbers from 1 to %d is %lld\n", n, global_sum);
    }

    double end_time = MPI_Wtime();

    // printf("Time taken: %f\n", end_time - start_time);
    // cout << "Time taken: " <<  << endl;

    // Finalize the MPI environment
    MPI_Finalize();

    if (rank == 0)
        printf("Time taken: %f\n", end_time - start_time);

    return 0;
}
