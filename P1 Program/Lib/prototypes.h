#include <stdio.h>
#include <stdlib.h>
#ifndef PROTOTYPE_H_
#define PROTOTYPE_H_

void dynamicFileTable(FILE *f, int *rows, int *cols, int isSym);
void loadSynListe(FILE *file, int row3, int col3, char *arr);
void ConstructArray(int rows, int cols, FILE *file, char *arr);
void printArr(int rows, int cols, char *arr, char *name);
double compareFiles(int row1, int row2, int row3, int col1, int col2, int col3, char *arr1, char *arr2, char *Syn);
void removePunct(char *arr, int col, int i);
int SynCheck(char *ord, char *ord2, int row3, int col3, char *SynListe);

#endif