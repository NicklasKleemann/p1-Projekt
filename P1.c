#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int ConstructArray(char arr[][20], FILE *file)
{
    char c;
    int i, j = 0;
    for (i = 0; i < 128 && !feof(file); ++i)
    {
        do
        {
            c = fgetc(file);
            if (isspace(c) || c == EOF) /* Optimering gjort her, uden c == EOF, -0\ i sidste ord og kan derfor ikke bruges som string */
            {
                if (c == '\n' && j < 19)
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
        } while (j < 20 && !isspace(c) && c != EOF);
        j = 0;
    }
    return i;
}
void printArr(char arr[][20], int stop_i, char *name) // PRINT INPUT FILE AND CONTROL FILE
{
    int i;
    printf("\033[1;33m%s\033[0m:\n", name); // NAME OF EACH FILE
    for (i = 0; i < 128 && i != stop_i; ++i)
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

    while (strstr(SynListe[line_number], ord) == NULL && line_number < 128)
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

double compareFiles(char arr1[][20], char arr2[][20], int EOT1, int EOT2, char Syn[][256])
{
    printf("\nComparing files...\n");
    int counter = 0, syncounter = 0;
    int j = 0;

    for (int i = 0; i < EOT1 && i < EOT2; ++i) // REMOVE PUNCTUATION FROM WORDS BEFORE COMPARING
    {
        for (j = 0; arr1[i][j] != '\0'; j++)
        {
            if (ispunct(arr1[i][j]) || arr1[i][j] == '\n')
            {
                arr1[i][j] = '\0';
            }
        }

        for (j = 0; arr2[i][j] != '\0'; j++)
        {
            if (ispunct(arr2[i][j]) || arr2[i][j] == '\n')
            {
                arr2[i][j] = '\0';
            }
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

int main(int argc, char **argv)
{
    int EOT1 = 0, EOT2 = 0, EOC = 0; // End OF text 1 & 2 and End of comparison.

    // FILE COMPARE
    FILE *Test = fopen("t1.txt", "r");
    FILE *Kontrol = fopen("t2.txt", "r");
    FILE *SynListe = fopen("SynListe.txt", "r"); // Synonym liste

    char text1[128][20]; // Mængde af ord og max længde på ord
    char text2[128][20];
    char Synonymer[128][256];

    loadSynListe(SynListe, Synonymer);

    printf("\n##############################################################################\n");
    EOT1 = ConstructArray(text1, Test);
    printArr(text1, EOT1, "\nInput text");
    EOT2 = ConstructArray(text2, Kontrol);
    printArr(text2, EOT2, "Kontrol text");

    if (EOT1 > EOT2)
    {
        EOC = EOT2;
    }
    else
    {
        EOC = EOT1;
    }

    double matches = compareFiles(text1, text2, EOT1, EOT2, Synonymer);
    double plagiat = matches / EOC * 100;
    printf("\nComparision between the files: We found matches on \033[1;34m%.0lf \033[0;34mout of \033[1;34m%d\033[0m words.\nResulting in a plagerize score of: \033[4;31;1m%lf%%\033[0m\n", matches, EOC, plagiat);
    printf("\n##############################################################################\n");
    fclose(Test);
    fclose(Kontrol);
    fclose(SynListe);

    return 0;
}