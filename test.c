#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void Dynamic(FILE *fp, int *rows, int *cols)
{
    int counter = 0;
    char c;
    while (!feof(fp))
    {
        printf("ydre");
        do{
            c = fgetc(fp);
            counter++;
            if (counter > *cols)
            {
                *cols = counter;
            }
        }while (!isspace(c) && c != EOF);

        *rows +=1; 
        counter = 0;
    }

    rewind(fp);
}

void main()
{
    int rows = 0, cols = 0, rows2 = 0, cols2 = 0;
    FILE *f = fopen("t1.txt", "r");
    FILE *fp = fopen("t2.txt", "r");
    
    if(f == NULL || fp == NULL){
        perror("ERROR :)");
    }

    Dynamic(f, &rows, &cols);
    Dynamic(fp, &rows2, &cols2);

    printf("\n\n%d & %d", rows, cols);
    printf("\n\n%d & %d", rows2, cols2);

    fclose(f);
}