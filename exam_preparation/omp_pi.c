#include <stdio.h>
#include <limits.h>
#include <omp.h>

int intervals = 1000;

double calc(double x)
{
    return 4.0 / (1 + x * x);
}

int main()
{
    double pi_final = 0.0, sum = 0.0;
    double h = 1.0 / intervals;

    double pi = 3.141592;

#pragma omp parallel for reduction(+ : sum)
    for (int i = 1; i <= intervals; i++)
    {
        double x = h * ((double)i - 0.5);
        sum += calc(x);
    }

    pi_final = h * sum;

    printf("The value of pi is: %lf\n", pi_final);
    printf("The error is: %lf\n", pi - pi_final);

    return 0;
}