#include <stdio.h>
#include <omp.h>
#include <limits.h>

int ARRAY_SIZE = 6;

int main(int argc, char const *argv[])
{
    int array[ARRAY_SIZE], maxi = INT_MIN;
    printf("The array is: ");
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100;
        printf("%d ", array[i]);
    }
    printf("\n");

#pragma omp parallel for reduction(max : maxi)
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (array[i] > maxi)
#pragma omp critical
        {
            maxi = array[i];
        }
    }

    printf("\nThe largest element is: %d\n", maxi);

    // = {2, 8, 10, 5, 3, 9};

    return 0;
}
