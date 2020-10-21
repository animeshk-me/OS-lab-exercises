#include "func_header.h"



/*********************** Multithreading Setup ************************/

// Returns length of longest common subsequence using Multithreading
int LCS(char * str1, char * str2) {
  // setting up arguments to be passed on to the threads
  struct block row_args, col_args; 
  strcpy(row_args.str1, str1);
  strcpy(row_args.str2, str2);
  col_args = row_args;
  memset(Cache, -1, sizeof(Cache));
  Initialize();

  int i = len1 - 1;
  int j = len2 - 1;

  // while you exhaust either the width or the length of the Cache
  while((i >= 0) && (j >= 0)) {
    Cache[i][j] = FillCell(i, j, str1, str2);
    row_args.id = i;
    row_args.end = j - 1;
    col_args.id = j;
    col_args.end = i - 1;
    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid1, &attr, runner_row, &row_args);
    pthread_create(&tid2, &attr, runner_col, &col_args);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    i--;
    j--;    
  }
  return Cache[0][0];
}

// The thread routine required to fill a single row
void *runner_row(void* params) {
  struct block * args = params;
  for (int i = args->end; i >= 0; i--)
    FillCell(args->id, i, args->str1, args->str2);
  pthread_exit(NULL);
}

// the thread routine required to fill a single column
void *runner_col(void *params) {
  struct block * args = params;
  for (int j = args->end; j >= 0; j--) 
    FillCell(j, args->id, args->str1, args->str2);
  pthread_exit(NULL);
}

// Fills the Cell (i, j) appropriately
int FillCell(int x, int y, char * str1, char * str2) {
  if(str1[x] == str2[y])
    Cache[x][y] = 1 + Cache[x + 1][y + 1];
  else
    Cache[x][y] = maximum(Cache[x + 1][y], Cache[x][y + 1]);
  return Cache[x][y];
}


// Initialize the Cache table with base cases
void Initialize() {
  for (int i = 0; i <= len1; i++)
    Cache[i][len2] = 0;
  for (int i = 0; i <= len2; i++)
    Cache[len1][i] = 0;
}

/********************************************************************/



/************************ General Functions *************************/


// Returns the MAX of n1 and n2
int maximum(int n1, int n2) {
  if (n1 > n2)
    return n1;
  return n2;
}

// Take input string in 'str' from stdin
void get_input(char * str, char * msg) {
  printf("Enter the %s string: \n", msg);
  fgets(str, SIZE, stdin);
  RipOffNextLine(str);
}

// A simple utility to rip off the trailing 'nextline' character from 'str'
void RipOffNextLine(char * str) {
  int i = 0;
  while(str[i] != '\n') 
    i++;
  str[i] = '\0';
}

/********************************************************************/
