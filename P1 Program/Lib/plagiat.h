#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printArr(int rows, int cols, char *arr, char *name) // PRINT INPUT FILE AND CONTROL FILE
{
    int i;
    printf("\033[1;33m%s\033[0m:\n", name); // NAME OF EACH FILE
    for (i = 0; i < rows; ++i)
    {
        printf(" ");
        printf("%s", arr + i * cols);
    }
    printf("\n");
}

double compareFiles(int row1, int row2, int row3, int col1, int col2, int col3, char *arr1, char *arr2, char *Syn)
{
    printf("\nComparing files...\n");
    int counter = 0, syncounter = 0;
    int j = 0;

    for (int i = 0; i < row1 && i < row2; ++i)
    {
        removePunct(arr1, col1, i);
        removePunct(arr2, col2, i);

        if (strcmp(arr1 + i * col1, arr2 + i * col2) == 0) // CHECK SIMILARITY
        {
            counter++;
        }
        else
        {
            if (SynCheck(arr1 + i * col1, arr2 + i * col2, row3, col3, Syn) == 1)
            { // CHECK SYNONYM EVERY DIFFERENCE
                counter++;
                syncounter++;
                printf("\033[1;32m%s\033[0;32m and\033[1;32m %s\033[0;32m are synonyms\033[0m\n", arr1 + i * col1, arr2 + i * col2);
            }
        }
    }
    printf("\nFound a total of\033[1;32m %d\033[0m synonym exchanges", syncounter);
    return counter;
}

void removePunct(char *arr, int col, int i)
{
    // REMOVE PUNCTUATION FROM WORDS BEFORE COMPARING
    if (ispunct(*(arr + i * col + strlen(arr + i * col) - 1)) || *(arr + i * col + strlen(arr + i * col) - 1) == '\n')
    {
        *(arr + i * col + strlen(arr + i * col) - 1) = '\0';
    }
}

int SynCheck(char *ord, char *ord2, int row3, int col3, char *SynListe)
{
    int line_number = 0;
    // Find "ord" fra Synonymlisten
    while (strstr(SynListe + line_number * col3, ord) == NULL && line_number < row3)
    { 
        line_number++;
    }

    // Findes "ord2" på matchende indeks af synonymlisten?
    return (strstr(SynListe + line_number * col3, ord2) != NULL) ? 1 : 0;
}