#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    int num = 1000000, i;
    int c;

    srand(time(NULL));
    c = rand();

    FILE *fptr;
    fptr = fopen("input.txt", "w");

    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    for (i = 0; i <= num; i++)
    {
        c = rand() % 1000000;
        fprintf(fptr, "%d ", c);
    }
    fclose(fptr);

    return 0;
}