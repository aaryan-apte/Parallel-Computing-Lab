#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    unsigned long long int n = 5, fac = 1;

#pragma omp parallel for reduction(* : fac)
    for (int i = 2; i <= n; i++)
    {
        fac = fac * i;
    }

    printf("%lld\n", fac);

    return 0;
}
