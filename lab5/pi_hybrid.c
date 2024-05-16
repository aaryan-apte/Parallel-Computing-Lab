#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <sys/time.h>
#include <stdint.h>

// NPTS is the number of intervals to do the integration..
#define NPTS 10000000000

// NLOOP is the number of loops for calculation
#define NLOOP 1

void main(int argc, char **argv)
{
    uint32_t rank, nproc;
    uint64_t i, j, istart, iend, start_time, end_time;
    struct timeval start, end;
    double sum, pi, mflops, max;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    max = 0;
    if (rank == 0)
    {
        istart = 1 + rank * NPTS;
        iend = (rank + 1) * NPTS;
        for (j = 0; j < NLOOP; ++j)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            gettimeofday(&start, NULL);
            sum = 0.0;
#pragma omp parallel for reduction(+ : sum)
            for (i = istart; i <= iend; ++i)
                sum += 0.5 / ((i - 0.75) * (i - 0.25));
            MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
            gettimeofday(&end, NULL);
            start_time = start.tv_sec * 1e6 + start.tv_usec;
            end_time = end.tv_sec * 1e6 + end.tv_usec;
            mflops = nproc * NPTS * (5.0 / (end_time - start_time));
            printf("processes = %d, threads = %d, NPTS = %ld, pi = %f\n", nproc, omp_get_max_threads(), NPTS, pi);
            if (mflops > max)
                max = mflops;
            printf("time = %f, estimated MFlops = %f, max = %f\n", (end_time - start_time) / 1.0e6, mflops, max);
        }
    }
    else
    {
        istart = 1 + rank * NPTS;
        iend = (rank + 1) * NPTS;
        for (j = 0; j < NLOOP; ++j)
        {
            MPI_Barrier(MPI_COMM_WORLD);
            sum = 0.0;
#pragma omp parallel for reduction(+ : sum)
            for (i = istart; i <= iend; ++i)
                sum += 0.5 / ((i - 0.75) * (i - 0.25));
            MPI_Reduce(&sum, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}