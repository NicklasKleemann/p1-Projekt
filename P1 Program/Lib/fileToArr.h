#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void dynamicFileTable(FILE *f, int *rows, int *cols, int isSym)
{
    int counter = 0;
    char c;
    c = fgetc(f);
    while (c != EOF)
    {
        counter++;
        if (counter > *cols)
        {
            *cols = counter;
        }

        if (isspace(c) && isSym == 0)
        {
            counter = 0;
            *rows += 1;
        }
        if (c == '\n' && isSym == 1)
        {
            counter = 0;
            *rows += 1;
        }
        c = fgetc(f);
    }
    // printf("\n%d rows \n%d cols\n", *rows, *cols); //USE FOR DEBUG
    rewind(f);
}

void loadSynListe(FILE *file, int col3, char *arr)
{ // LOAD SYNONYM LIST INTO ARRAY
    int line = 0;
    while (!feof(file) && !ferror(file))
    {
        if (fgets(arr + line * col3, col3 + 1, file) != NULL)
        {
            line++;
        }
    }
}

void ConstructArray(int rows, int cols, FILE *file, char *arr)
{
    char c;
    int i, j = 0;
    for (i = 0; i < rows && !feof(file); ++i)
    {
        do
        {
            c = fgetc(file);
            if (isspace(c) || c == EOF)
            {
                if (c == '\n' && j < cols)
                {
                    *(arr + i * cols + j) = c;
                    *(arr + i * cols + j + 1) = '\0';
                }
                else
                {
                    *(arr + i * cols + j) = '\0';
                }
            }
            else
            {
                *(arr + i * cols + j) = c;
            }

            j++;
        } while (j < cols && !isspace(c) && c != EOF);
        j = 0;
    }
}