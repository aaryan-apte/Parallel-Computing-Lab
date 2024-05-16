#include <stdio.h>
#include <omp.h>

int array_size = 100;

int main(int argc, char const *argv[])
{
    omp_set_num_threads(10);
    int array[array_size];
    for (int i = 0; i < array_size; i++)
    {
        array[i] = i + 1;
    }

    int sum = 0;

#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < array_size; i++)
    {
        sum += array[i];
    }

    printf("The sum is: %d\n", sum);

    return 0;
}
