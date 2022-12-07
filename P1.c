#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void ConstructArray(int rows, int cols, FILE *file, char arr[][cols])
{
    char c;
    int i, j = 0;
    for (i = 0; i < rows && !feof(file); ++i)
    {
        do
        {
            c = fgetc(file);
            if (isspace(c) || c == EOF) /* Optimering gjort her, uden c == EOF, -0\ i sidste ord og kan derfor ikke bruges som string */
            {
                if (c == '\n' && j < cols)
                {
                    arr[i][j] = c;
                    arr[i][j + 1] = '\0';
                }
                else
                {
                    arr[i][j] = '\0';
                }
            }
            else
            {
                arr[i][j] = c;
            }

            j++;
        } while (j < cols && !isspace(c) && c != EOF);
        j = 0;
    }
}

void printArr(int rows, int cols, char arr[][cols], char *name) // PRINT INPUT FILE AND CONTROL FILE
{
    int i;
    printf("\033[1;33m%s\033[0m:\n", name); // NAME OF EACH FILE
    for (i = 0; i < rows; ++i)
    {
        printf(" ");
        printf("%s", arr[i]);
    }
    printf("\n");
}

void loadSynListe(FILE *file, char arr[][256])
{ // LOAD SYNONYM LIST INTO ARRAY
    int line = 0;
    while (!feof(file) && !ferror(file))
    {
        if (fgets(arr[line], 256, file) != NULL)
        {
            line++;
        }
    }
}

int SynCheck(char *ord, char *ord2, char SynListe[][256])
{
    int line_number = 0;

    while (strstr(SynListe[line_number], ord) == NULL && line_number < 2)
    { // FIND "ord" FROM SYNLISTE
        line_number++;
    }

    if (strstr(SynListe[line_number], ord2) != NULL)
    { // FIND "ord2" FROM SYNLISTE WHICH MATCHES THE SAME INDEX AS "ord"
        return 1;
    }
    else
    {
        return 0;
    }
}

double compareFiles(int row1, int row2, int col1, int col2, char arr1[row1][col1], char arr2[row2][col2], char Syn[][256])
{
    printf("\nComparing files...\n");
    int counter = 0, syncounter = 0;
    int j = 0;

    for (int i = 0; i < 8; ++i) // REMOVE PUNCTUATION FROM WORDS BEFORE COMPARING
    {
        if (ispunct(arr1[i][strlen(arr1[i]) - 1]) || arr1[i][strlen(arr1[i]) - 1] == '\n')
        {
            arr1[i][strlen(arr1[i]) - 1] = '\0';
        }
        if (ispunct(arr2[i][strlen(arr2[i]) - 1]) || arr2[i][strlen(arr2[i]) - 1] == '\n')
        {
            arr2[i][strlen(arr2[i]) - 1] = '\0';
        }

        if (strcmp(arr1[i], arr2[i]) == 0) // CHECK SIMILARITY
        {
            counter++;
        }
        else
        {
            if (SynCheck(arr1[i], arr2[i], Syn) == 1)
            { // CHECK SYNONYM EVERY DIFFERENCE
                counter++;
                syncounter++;
                printf("\033[1;32m%s\033[0;32m and\033[1;32m %s\033[0;32m are synonyms\033[0m\n", arr1[i], arr2[i]);
            }
        }
    }
    printf("\nFound a total of\033[1;32m %d\033[0m synonym exchanges", syncounter);
    return counter;
}

void dynamicFileTable(FILE *f, int *rows, int *cols, int isSym)
{
    int counter = 0;
    char ch = fgetc(f);
    while (ch != EOF)
    {
        counter++;

        if (counter > *cols)
        {
            *cols = counter;
        }

        switch (isSym)
        {
        case 0:
            if (isspace(ch))
            {
                counter = 0;
                *rows +=1;
            }
            break;
        case 1:
            if (ch == '\n')
            {
                counter = 0;
                *rows +=1;
            }
            break;
        }
        ch = fgetc(f);
    }

    printf("\n%d rows \n%d cols\n", *rows, *cols);
    rewind(f);
}

int main(int argc, char **argv)
{
    int EOC = 0; // End OF text 1 & 2 and End of comparison.
    int row1 = 1, row2 = 1, row3 = 1;
    int col1 = 1, col2 = 1, col3 = 1;

    // FILE COMPARE
    FILE *Test = fopen("t1.txt", "r");
    FILE *Kontrol = fopen("t2.txt", "r");
    FILE *SynListe = fopen("SynListe.txt", "r"); // Synonym liste

    if (Test == NULL || Kontrol == NULL || SynListe == NULL)
    {
        perror("FILE ERROR\n");
        exit(-1);
    }

    dynamicFileTable(Test, &row1, &col1, 0);
    dynamicFileTable(Kontrol, &row2, &col2, 0);
    dynamicFileTable(SynListe, &row3, &col3, 1);

    char text1[row1 + 1][col1 + 1]; // Mængde af ord og max længde på ord
    char text2[row2 + 1][col2 + 1];
    char Synonymer[row3 + 1][col3 + 1];

    loadSynListe(SynListe, Synonymer);
    printf("\n##############################################################################\n");
    ConstructArray(row1, col1, Test, text1);
    printArr(row1, col1, text1, "\nInput text");
    ConstructArray(row2, col2, Kontrol, text2);
    printArr(row2, col2, text2, "Kontrol text");

    if (row1 > row2)
    {
        EOC = row2;
    }
    else
    {
        EOC = row1;
    }

    double matches = compareFiles(row1, row2, col1, col2, text1, text2, Synonymer);
    double plagiat = matches / EOC * 100;
    printf("\nComparision between the files: We found matches on \033[1;34m%.0lf \033[0;34mout of \033[1;34m%d\033[0m words.\nResulting in a plagerize score of: \033[4;31;1m%lf%%\033[0m\n", matches, EOC, plagiat);
    printf("\n##############################################################################\n");
    fclose(Test);
    fclose(Kontrol);
    fclose(SynListe);

    return 0;
}