#include <stdio.h>
#include <omp.h>
int main()
{
    int numberOfElements, currentMax = -1, iIterator, arrayInput[10000];
    printf("Enter the Number of Elements: ");
    scanf("%d", &numberOfElements);

    for (iIterator = 0; iIterator < numberOfElements; iIterator++)
    {
        scanf("%d", &arrayInput[iIterator]);
    }

#pragma omp parallel for shared(currentMax)
    for (iIterator = 0; iIterator < numberOfElements; iIterator++)
    {
#pragma omp critical
        if (arrayInput[iIterator] > currentMax)
        {
            currentMax = arrayInput[iIterator];
        }
    }

    printf("The Maximum Element is: %d\n", currentMax);
    return 0;
}