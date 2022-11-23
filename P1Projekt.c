#include <stdio.h>
#include <sqlite-amalgamation-3400000\sqlite3.h> 

int main(int argc, char **argv){
    sqlite3 *db;
    char *errmsg = 0;

    int database = sqlite3_open("Database/SynListe.db",&db);
    

    return 1;
}