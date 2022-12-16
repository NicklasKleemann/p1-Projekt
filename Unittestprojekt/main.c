#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CuTest.h"
#include "functions.h"

int main(void)
{
    int EOC = 0; // End OF text 1 & 2 and End of comparison.
    int row1 = 1, row2 = 1, row3 = 1;
    int col1 = 1, col2 = 1, col3 = 1;

    // FILE COMPARE
    FILE *Test = fopen("InputFiles/t1.txt", "r");
    FILE *Kontrol = fopen("InputFiles/t2.txt", "r");
    FILE *SynListe = fopen("InputFiles/SynListe.txt", "r"); // Synonym liste

    if (Test == NULL || Kontrol == NULL || SynListe == NULL)
    {
        perror("FILE ERROR\n");
        exit(-1);
    }

    dynamicFileTable(Test, &row1, &col1, 0);
    dynamicFileTable(Kontrol, &row2, &col2, 0);
    dynamicFileTable(SynListe, &row3, &col3, 1);

    char *text1 = malloc(sizeof(char) * row1 * col1); // Mængde af ord og max længde på ord
    char *text2 = malloc(sizeof(char) * row2 * col2);
    char *Synonymer = malloc(sizeof(char) * row3 * col3);

    loadSynListe(SynListe, row3, col3, Synonymer);
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

    double matches = compareFiles(row1, row2, row3, col1, col2, col3, text1, text2, Synonymer);
    double plagiat = matches / EOC * 100;
    printf("\nComparision between the files: We found matches on \033[1;34m%.0lf \033[0;34mout of \033[1;34m%d\033[0m words.\nResulting in a plagerize score of: \033[4;31;1m%lf%%\033[0m\n", matches, EOC, plagiat);
    printf("\n##############################################################################\n");
    fclose(Test);
    fclose(Kontrol);
    fclose(SynListe);

    free(text1);
    free(text2);
    free(Synonymer);

    return 0;
}



