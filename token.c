#include <stdio.h>
#include <string.h>
 
int main()
{
    char str[] = "daarlig grusom elendig skraekelig haabloes ringe underlegen utilstraekelig Daarlig Grusom Elendig Skraekelig Haabloes Ringe Underlegen Utilstraekelig";
 
    // Returns first token
    char* token = strtok(str, " ");
 
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
        printf(" % s\n", token);
        token = strtok(NULL, " ");
    }
 
    return 0;
}