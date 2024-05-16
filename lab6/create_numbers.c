#include <stdio.h>
// #include <conio.h>
#include <time.h>
#include <stdlib.h>

void main()
{
    FILE *f;
    int c;
    // clrscr();

    srand(time(NULL));
    c = rand();

    if ((f = fopen("input.txt", "w")) == NULL)
        printf("Could not open file");
    for (int i = 0; i < 1000000; i++)
    {
        c = rand();
        putc(c, f);
    }
    fclose(f);
}