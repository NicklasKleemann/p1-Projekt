
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "CuTest.h"
#include "functions.h"

int SynCheckTest(char *ord, char *ord2, int row3, int col3, char *SynListe);

void dynamicFileTable(FILE *f, int *rows, int *cols, int isSym)
{
    int counter = 0;
    char c;
    c = fgetc(f);
    while (c != EOF)
    {
        counter++;
        if (counter > *cols)
        {
            *cols = counter;
        }

        if (isspace(c) && isSym == 0)
        {
            counter = 0;
            *rows += 1;
        }
        if (c == '\n' && isSym == 1)
        {
            counter = 0;
            *rows += 1;
        }
        c = fgetc(f);
    }
    // printf("\n%d rows \n%d cols\n", *rows, *cols); //USE FOR DEBUG
    rewind(f);
}

void loadSynListe(FILE *file, int row3, int col3, char *arr)
{ // LOAD SYNONYM LIST INTO ARRAY
    int line = 0;
    while (!feof(file) && !ferror(file))
    {
        if (fgets(arr + line * col3, col3 + 1, file) != NULL)
        {
            line++;
        }
    }
}

void ConstructArray(int rows, int cols, FILE *file, char *arr)
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
                    *(arr + i * cols + j) = c;
                    *(arr + i * cols + j + 1) = '\0';
                }
                else
                {
                    *(arr + i * cols + j) = '\0';
                }
            }
            else
            {
                *(arr + i * cols + j) = c;
            }
            j++;
        } while (j < cols && !isspace(c) && c != EOF);
        j = 0;
    }
}

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
        printf("arr1[%d] = %s, arr2[%d] = %s\n", i, arr1 + i * col1, i, arr2 + i * col2);
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


/******* STUB ********/
int SynCheck(char *ord, char *ord2, int row3, int col3, char *SynListe) {

  return 0;
}



/******* TESTS *******/

/******* Test af compareFiles funktionen ********/

void test_compareFiles_identical_arrays_of_words(CuTest *tc){
  printf("123");
  char *arr1 = malloc(sizeof(char) * 8 * 8); 
  char *text1[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 8);
  char *text2[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 8, text2[i]);
  } 
  char *syn[] = {};

  double actual = compareFiles(8,8,2,8,8,10,arr1,arr2,&syn[0][0]);
  double expected = 8;

  CuAssertDblEquals(tc, expected, actual, 0.00001);

  free(arr1);
  free(arr2);
}

void test_compareFiles_arrays_of_different_length(CuTest *tc){
  char *arr1 = malloc(sizeof(char) * 9 * 8);
  char *text1[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program", "end"};
  for(int i = 0; i < 9; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 8);
  char *text2[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 8, text2[i]);
  }
  char *syn[] = {};
  double actual = compareFiles(9,8,2,8,8,10,arr1,arr2,&syn[0][0]);
  double expected = 8;

  CuAssertDblEquals(tc, expected, actual, 0.00001);
  free(arr1);
  free(arr2);
}

void test_compareFiles_arrays_of_different_words(CuTest *tc){
  char *arr1 = malloc(sizeof(char) * 8 * 8);
  char *text1[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program"}; 
  for(int i = 0; i < 8; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 8);
  char *text2[] = {"idag", "sner", "det", "udenfor", "og", "er", "meget", "koldt"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 8, text2[i]);
  }
  char *syn[] = {};
  double actual = compareFiles(8,8,2,8,8,10,arr1,arr2,&syn[0][0]);
  double expected = 0;
  CuAssertDblEquals(tc, expected, actual, 0.00001);
  free(arr1);
  free(arr2);
}

void test_compareFiles_arrays_some_equal_words(CuTest *tc){
  char *arr1 = malloc(sizeof(char) * 8 * 8);
  char *text1[] = {"hej", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 8);
  char *text2[] = {"hej", "dette", "er", "en", "test", "af", "et", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 8, text2[i]);
  }
  char *syn[] = {};
  double actual = compareFiles(8,8,2,8,8,10,arr1,arr2,&syn[0][0]);
  double expected = 6;

  CuAssertDblEquals(tc, expected, actual, 0.00001);
  free(arr1);
  free(arr2);
}

void test_compareFiles_arrays_with_staggered_index(CuTest *tc){
  char *arr1 = malloc(sizeof(char) * 9 * 8);
  char *text1[] = {"jamen", "hej", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 9; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 15);
  char *text2[] = {"hej", "dette", "er", "en", "test", "af", "compareFiles", "funktionalitet"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 15, text2[i]);
  }
  char *syn[] = {};
  double actual = compareFiles(9,8,2,8,15,10,arr1,arr2,&syn[0][0]);
  double expected = 0;

  CuAssertDblEquals(tc, expected, actual, 0.00001);
  free(arr1);
  free(arr2);
}

void test_compareFiles_arrays_with_punctuation(CuTest *tc){
  char *arr1 = malloc(sizeof(char) * 8 * 8); 
  char *text1[] = {"hej,", "dette", "er", "en", "unitest", "af", "vores", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr1 + i * 8, text1[i]);
  }
  char *arr2 = malloc(sizeof(char) * 8 * 8);
  char *text2[] = {"hej", "dette", "er", "en", "unitest", "af", "vores!", "program"};
  for(int i = 0; i < 8; i++) {
    strcpy(arr2 + i * 8, text2[i]);
  } 
  char *syn[] = {};

  double actual = compareFiles(8,8,2,8,8,10,arr1,arr2,&syn[0][0]);
  double expected = 8;

  CuAssertDblEquals(tc, expected, actual, 0.00001);

  free(arr1);
  free(arr2);
}

/******* Test af SynCheck funktionen ********/
int SynCheckTest(char *ord, char *ord2, int row3, int col3, char *SynListe)
{
    int line_number = 0;
    while (strstr(SynListe + line_number * col3, ord) == NULL && line_number < row3)
    { // FIND "ord" FROM SYNLISTE
        line_number++;
    }

    if (strstr(SynListe + line_number * col3, ord2) != NULL)
    { // FIND "ord2" FROM SYNLISTE WHICH MATCHES THE SAME INDEX AS "ord"
        return 1;
    }
    else
    {
        return 0;
    }
}


void test_SynCheck_synonyms_passed(CuTest *tc){
  char *SynListe = malloc (sizeof(char) * 2 * 186);
  char *arr[] = {"fantastisk glimrende fremragende fortreaffeligt fabelagtigt eventyrligt enestaaende brillant Fantastisk Glimrende Fremragende Fortreaffeligt Fabelagtigt Eventyrligt Enestaaende Brillant", "daarlig grusom elendig skraekelig haabloes ringe underlegen utilstraekelig Daarlig Grusom Elendig Skraekelig Haabloes Ringe Underlegen Utilstraekelig"};
  for(int i = 0; i < 2; i++) {
    strcpy(SynListe + i * 186, arr[i]);
  } 
  char *ord1 = "fantastisk";
  char *ord2 = "enestaaende";
  int actual = SynCheckTest(ord1, ord2, 2, 186, SynListe);
  int expected = 1;

  CuAssertIntEquals(tc, expected, actual);

  free(SynListe);
}

void test_SynCheck_words_not_in_list_passed(CuTest *tc){
  char *SynListe = malloc (sizeof(char) * 2 * 186);
  char *arr[] = {"fantastisk glimrende fremragende fortreaffeligt fabelagtigt eventyrligt enestaaende brillant Fantastisk Glimrende Fremragende Fortreaffeligt Fabelagtigt Eventyrligt Enestaaende Brillant", "daarlig grusom elendig skraekelig haabloes ringe underlegen utilstraekelig Daarlig Grusom Elendig Skraekelig Haabloes Ringe Underlegen Utilstraekelig"};
  for(int i = 0; i < 2; i++) {
    strcpy(SynListe + i * 186, arr[i]);
  } 
  char *ord1 = "skole";
  char *ord2 = "kirke";
  int actual = SynCheckTest(ord1, ord2, 2, 186, SynListe);
  int expected = 0;

  CuAssertIntEquals(tc, expected, actual);

  free(SynListe);
}

void test_SynCheck_1_in_list_1_not_in_list(CuTest *tc){
  char *SynListe = malloc (sizeof(char) * 2 * 186);
  char *arr[] = {"fantastisk glimrende fremragende fortreaffeligt fabelagtigt eventyrligt enestaaende brillant Fantastisk Glimrende Fremragende Fortreaffeligt Fabelagtigt Eventyrligt Enestaaende Brillant", "daarlig grusom elendig skraekelig haabloes ringe underlegen utilstraekelig Daarlig Grusom Elendig Skraekelig Haabloes Ringe Underlegen Utilstraekelig"};
  for(int i = 0; i < 2; i++) {
    strcpy(SynListe + i * 186, arr[i]);
  } 
  char *ord1 = "daarlig";
  char *ord2 = "doven";
  int actual = SynCheckTest(ord1, ord2, 2, 186, SynListe);
  int expected = 0;

  CuAssertIntEquals(tc, expected, actual);

  free(SynListe);
}

void test_SynCheck_both_in_list_not_synonyms(CuTest *tc){
  char *SynListe = malloc (sizeof(char) * 2 * 186);
  char *arr[] = {"fantastisk glimrende fremragende fortreaffeligt fabelagtigt eventyrligt enestaaende brillant Fantastisk Glimrende Fremragende Fortreaffeligt Fabelagtigt Eventyrligt Enestaaende Brillant", "daarlig grusom elendig skraekelig haabloes ringe underlegen utilstraekelig Daarlig Grusom Elendig Skraekelig Haabloes Ringe Underlegen Utilstraekelig"};
  for(int i = 0; i < 2; i++) {
    strcpy(SynListe + i * 186, arr[i]);
  } 
  char *ord1 = "daarlig";
  char *ord2 = "fantastisk";
  int actual = SynCheckTest(ord1, ord2, 2, 186, SynListe);
  int expected = 0;

  CuAssertIntEquals(tc, expected, actual);

  free(SynListe);
}






/****** SUIT *******/

CuSuite *compareFilesGetSuite() {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_compareFiles_identical_arrays_of_words);
  SUITE_ADD_TEST(suite, test_compareFiles_arrays_of_different_length);
  SUITE_ADD_TEST(suite, test_compareFiles_arrays_of_different_words); 
  SUITE_ADD_TEST(suite, test_compareFiles_arrays_some_equal_words); 
  SUITE_ADD_TEST(suite, test_compareFiles_arrays_with_staggered_index); 
  SUITE_ADD_TEST(suite, test_compareFiles_arrays_with_punctuation);
   // NEW
  //TODO: Add more tests

  return suite;
}

CuSuite *SynCheckGetSuite() {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, test_SynCheck_synonyms_passed);
  SUITE_ADD_TEST(suite, test_SynCheck_words_not_in_list_passed);
  SUITE_ADD_TEST(suite, test_SynCheck_1_in_list_1_not_in_list);
  SUITE_ADD_TEST(suite, test_SynCheck_both_in_list_not_synonyms);

   // NEW
  //TODO: Add more tests

  return suite;
}





