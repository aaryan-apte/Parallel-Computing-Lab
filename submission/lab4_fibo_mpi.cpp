#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

unsigned long long fibonacci(int n)
{
    if (n <= 1)
        return n;
    unsigned long long a = 0, b = 1;
    for (int i = 2; i <= n; ++i)
    {
        unsigned long long next = a + b;
        a = b;
        b = next;
    }
    return b;
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 50;
    const int chunk_size = N / size;

    vector<unsigned long long> local_results(chunk_size);

    for (int i = 0; i < chunk_size; ++i)
    {
        local_results[i] = fibonacci(rank * chunk_size + i);
    }

    vector<unsigned long long> global_results(N);
    MPI_Gather(&local_results[0], chunk_size, MPI_UNSIGNED_LONG_LONG, &global_results[0], chunk_size, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        cout << "Fibonacci Series: ";
        for (int i = 0; i < N; ++i)
        {
            cout << global_results[i] << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}
