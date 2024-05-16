#include <stdio.h>
#include <omp.h>
#include <limits.h>

int size = 5;

int main(int argc, char const *argv[])
{
    int max_number = INT_MIN;
    int array[size];

    for (int i = 0; i < size; i++)
    {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
    }
    printf("\n");

#pragma omp parallel reduction(max : max_number)
    for (int i = 0; i < size; i++)
    {
        if (array[i] > max_number)
#pragma omp critical
        {
            max_number = array[i];
        }
    }

    printf("The maximum value is: %d\n", max_number);

    return 0;
}
