#include <mpi.h>
#include <stdio.h>

double calculate_integration(double x)
{
    return 4.0 / (1 + x * x);
}

int main(int argc, char **argv)
{
    int rank, size, no_intervals, no_procs;
    double local_pi = 0.0, global_pi = 0.0;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        printf("Enter number of intervals: \n");
        scanf("%d", &no_intervals);
    }

    MPI_Bcast(&no_intervals, 1, MPI_INT, 0, MPI_COMM_WORLD);

    double h = 1.0 / no_intervals;
    double sum = 0.0;

    for (int interval = rank + 1; interval <= no_intervals; interval += no_procs)
    {
        double x = h * ((double)interval - 0.5);
        sum = sum + calculate_integration(x);
    }

    local_pi = sum * h;

    MPI_Reduce(&local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Finalize();

    if (rank == 0)
        printf("The value of calculated pi is: %lf\n", global_pi);

    return 0;
}