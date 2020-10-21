#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

#define SIZE 100

// A package to pass around data among threads
struct block {
  int id;   // row_id or col_id
  int end;  // index of end of row/column
  char str1[SIZE];  // first string
  char str2[SIZE];  // second string
};

int len1, len2;
int Cache[SIZE + 1][SIZE + 1]; // Cache table for dynamic programming

void Initialize();
void *runner_row(void* params);
void *runner_col(void* params);

int LCS(char * str1, char * str2);
int FillCell(int x, int y, char * str1, char * str2);

int maximum(int n1, int n2);
void get_input(char * str, char * msg);
void RipOffNextLine(char * str);