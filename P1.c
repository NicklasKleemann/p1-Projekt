#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void ConstructArray(char arr[][20], FILE *file, int *end_i){
    char c;
    fgetc(file);
    int j = 0;
    int i;
    printf("ConstructArray: ");
    for(i = 0 ; i < 128 && !feof(file); ++i){
        do{
            c = fgetc(file);
            if (c == ' ') {
                arr[i][j] = '\0';
            }
            else {arr[i][j] = c; 
            printf("%c", arr[i][j]);
            }
            j++;
        }  while (j < 20 && c != ' ');
        printf(" ");
        j = 0;
    }
    *end_i = i; 
}
void printArr(char arr[][20], int stop_i){
    int i, j;
    printf("printArr: ");
    for(i = 0; i <128 && i != stop_i; ++i){
        for (j= 0; j < 20 && arr[i][j] != '\0'; ++j){
            printf("%c", arr[i][j]);
        }
        if(j>1)printf(" ");
    }
    printf("\n");
}

int main(int argc, char **argv){
    int stop_i;
    FILE *Original;
    FILE *Kontrol;

    FILE *SynListe; //Synonym liste

    char text1[128][20]; //Mængde af ord og max længde på ord
    char text2[128][20];
    
    Original = fopen("t1.txt", "r");
    Kontrol = fopen("t2.txt", "r");
   

    ConstructArray(text1, Original, &stop_i);
    printArr(text1, stop_i);
    ConstructArray(text2, Kontrol, &stop_i);
    printArr(text2, stop_i);

    
    


    return 0;
}