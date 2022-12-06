#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int getFileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    int byteSize = ftell(fp);
    rewind(fp);

    return byteSize;
}

void DynamicSynonymsTable(FILE *synonyms, int *rows, int *cols)
{
    int counter = 0;
    char c;
    while (!feof(synonyms))
    {
        do
        {
            c = fgetc(synonyms);
            counter++;
            if (counter > *cols)
            {
                *cols = counter;
            }
        } while (c != '\n' && c != EOF);
        counter = 0;
        *rows += 1;
    }
    printf("\n%d rows \n%d cols\n", *rows, *cols);
    rewind(synonyms);
    
}

void loadSynListe(FILE *file, int cols, char syn[][cols])
{ // LOAD SYNONYM LIST INTO ARRAY
    int line = 0;
    while (!feof(file))
    {
        fgets(syn[line], cols, file);
        printf("%s\n", syn[line]);

        line += 1;
    }
    printf("FINITO");
}

int SynCheck(char *ord, char *ord2, int cols, char SynListe[][cols])
{
    int line_number = 0;

    while (strstr(SynListe[line_number], ord) == NULL)
    { // FIND "ord" FROM SYNLISTE
        line_number++;
        printf("\ninde i loop");
    }

    printf("i slut af syncheck");
    if (strstr(SynListe[line_number], ord2) != NULL)
    { // FIND "ord2" FROM SYNLISTE WHICH MATCHES THE SAME INDEX AS "ord"
        return 1;
    }
    else
    {
        return 0;
    }
 }   

double compareFiles(char text1[], char text2[], int *EOC, int cols, char Syn[][200])
{
    printf("\nComparing files...\n");

    int counter = 0, syncounter = 0;
    int j = 0;
    printf("text 1:%s", text1);
    printf("text 2:%s", text2);
    char *token1 = strtok(text1, " ");
    char *token2 = strtok(text2, " ");
    

    while (token1 != NULL && token2 != NULL) // REMOVE PUNCTUATION FROM WORDS BEFORE COMPARING
    {
        printf("\n%s\n%s",token1, token2);
        if (ispunct(token1[strlen(token1)]) || token1[strlen(token1)] == '\n')
        {
            token1[strlen(token1)] = '\0';
        }

        if (ispunct(token2[strlen(token2)]) || token2[strlen(token2)] == '\n')
        {
            token1[strlen(token2)] = '\0';
        }

        if (strcmp(token1, token2) == 0) // CHECK SIMILARITY
        {
            counter++;
        }
        else
        {
            if (SynCheck(token1, token2, cols, Syn) == 1)
            { // CHECK SYNONYM EVERY DIFFERENCE
                counter++;
                syncounter++;
                printf("\033[1;32m%s\033[0;32m and\033[1;32m %s\033[0;32m are synonyms\033[0m\n", token1, token2);
            }
        }
        EOC++;
        token1 = strtok(NULL, " ");
        token2 = strtok(NULL, " ");
    }
    printf("\nFound a total of\033[1;32m %d\033[0m synonym exchanges", syncounter);
    return counter;
}

int main()
{
    int rows = 0, cols = 0, fSize1 = 0, fSize2 = 0, EOC = 0;
    FILE *f = fopen("t1.txt", "r");
    FILE *fp = fopen("t2.txt", "r");
    FILE *SynListe = fopen("SynListe.txt", "r");

    if (f == NULL || fp == NULL || SynListe == NULL)
    {
        perror("ERROR :)");
    }

    // Store the size values
    fSize1 = getFileSize(f);
    fSize2 = getFileSize(fp);
    printf("%d and %d\n", fSize1, fSize2);

    char *arr1 = malloc((fSize1 + 1) * sizeof(char)); // DYNAMIC
    char *arr2 = malloc((fSize2 + 1) * sizeof(char)); 
    if (arr1 == NULL || arr2 == NULL)
    {
        perror("MALLOC ERROR :)");
    }
    fread(arr1, sizeof(char), fSize1, f);
    fread(arr2, sizeof(char), fSize2, fp);
    printf("%s\n", arr1);
    printf("%s\n", arr2);

    DynamicSynonymsTable(SynListe, &rows, &cols);
    char SynTable[rows][cols];
    loadSynListe(SynListe, cols, SynTable);
    printf("TEXT1 %s\n", arr1);
    printf("TEXT2 %s\n", arr2);

    double matches = compareFiles(arr1, arr2, &EOC, cols, SynTable);
    double plagiat = matches / EOC * 100;
    printf("\nComparision between the files: We found matches on \033[1;34m%.0lf \033[0;34mout of \033[1;34m%d\033[0m words.\nResulting in a plagerize score of: \033[4;31;1m%lf%%\033[0m\n", matches, EOC, plagiat);
    printf("\n##############################################################################\n");

    // free(arr1);
    // free(arr2);
    fclose(f);
    fclose(fp);
    fclose(SynListe);

    return 0;
}