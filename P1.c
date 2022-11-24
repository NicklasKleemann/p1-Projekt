#include <stdio.h>
#include <stdlib.h>

void ConstructArray(char ** arr, FILE * file){
    for( int i = 0 ; i < 128 ; ++i){
        for(int j = 0; j < 20 && fgetc(file)!=' ' && fgetc(file)!=EOF; ++j){
            arr[i][j] = fgetc(file);
            printf("%c", arr[i][j]);
        }
    }
}
void printArr(char ** arr){
    int i, j;
    for(i = 0; i <128; ++i){
        for (j= 0; j < 20; ++j){
            if(arr[i][j]==' ')break;
            printf("%c", arr[i][j]);
        }
        if(j>1)printf(" ");
    }
}
//HejS

int main(int argc, char **argv){
    FILE *Original;
    FILE *Kontrol;

    FILE *SynListe; //Synonym liste

    char original[128][20]; //Mængde af ord og max længde på ord
    char kontrol[128][20];
    
    Original = fopen("t1.txt", "r");
    Kontrol = fopen("t2.txt", "r");

    ConstructArray(original, Original);
    ConstructArray(kontrol, Kontrol);

    //printArr(original);
    //printArr(kontrol);


    return 0;
}