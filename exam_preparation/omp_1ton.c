#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 1000

int main()
{
    int n = ARRAY_SIZE;
    int sum = 0;

    // Initialize an array with numbers from 1 to n
    int numbers[n];
    for (int i = 0; i < n; i++)
    {
        numbers[i] = i + 1;
    }

#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; i++)
    {
        sum += numbers[i];
    }

    printf("The sum of the numbers from 1 to %d is: %d\n", n, sum);

    return 0;
}
