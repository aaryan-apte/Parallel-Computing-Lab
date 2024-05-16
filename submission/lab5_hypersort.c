#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>

#define MASTER 0

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

#pragma omp task
        quickSort(arr, low, pi - 1);

#pragma omp task
        quickSort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[])
{
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = 20;
    int arr[n];
    if (rank == MASTER)
    {
        srand(0);
        printf("Original array: ");
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % 100;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Bcast(arr, n, MPI_INT, MASTER, MPI_COMM_WORLD);

#pragma omp parallel
    {
#pragma omp single
        {
            quickSort(arr, 0, n - 1);
        }
    }

    if (rank == MASTER)
    {
        printf("Sorted array: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
