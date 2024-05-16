#include <iostream>
#include <mpi.h>
using namespace std;

bool isPrime(int n)
{
    if (n <= 1)
        return false;
    if (n <= 3)
        return true;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

int countPrimes(int start, int end)
{
    int count = 0;
    for (int i = start; i <= end; ++i)
    {
        if (isPrime(i))
        {
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n = 1000000;

    int local_start = rank * (n / size) + 1;
    int local_end = (rank + 1) * (n / size);

    if (rank == size - 1)
    {
        local_end = n;
    }

    int local_count = countPrimes(local_start, local_end);
    int total_count;

    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Total count of prime numbers up to " << n << ": " << total_count << endl;
    }

    MPI_Finalize();
    return 0;
}
