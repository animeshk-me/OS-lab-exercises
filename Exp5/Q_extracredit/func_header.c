#include "func_header.h"

#define SIZE 100

/**************** Multiprocessing Setup ************************/

// Returns length of longest common subsequence using Multiprocessing
int LCS(char * str1, char * str2) {
  int Cache[len1 + 1][len2 + 1];
  pid_t pid1, pid2, wpid;
  int pivot;
  int status = 0; 
  int fd1[2];
  int fd2[2];
  int fd3[2];
  int fd4[2];
  memset(Cache, -1, sizeof(Cache));
  Initialize(Cache);
  int i = len1 - 1;
  int j = len2 - 1;
  while((i >= 0) && (j >= 0)) {
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    (pid1 = fork()) && (pid2 = fork());
    if (pid1 == 0) {      // First Child Process
    // Take pivot from the parent
      close(fd3[1]); 
      read(fd3[0], &pivot, 4);
      close(fd3[0]);
      Cache[i][j] = pivot;
      FillRow(i, j - 1, str1, str2, Cache);
      SendRow(i, Cache, fd1);
      exit(0);
    }
    else if (pid2 == 0) { // Second Child Process
    // Take pivot from the parent
      close(fd4[1]);
      read(fd4[0], &pivot, 4);
      close(fd4[0]);
      Cache[i][j] = pivot;
      FillColumn(j, i - 1, str1, str2, Cache);
      SendColumn(j, Cache, fd2);
      exit(0);
    }
    else {          // Parent Process
      pivot = FillCell(i, j, str1, str2, Cache);
      // Send the calculated pivot to both the children
      close(fd3[0]);
      write(fd3[1], &pivot, 4);
      close(fd3[1]);
      close(fd4[0]);
      write(fd4[1], &pivot, 4);
      close(fd4[1]);

      while ((wpid = wait(&status)) > 0) // wait till all the children finish
      
      // Receive the modified row and column from the two children
      ReceiveRowAndColumn(i, j, Cache, fd1, fd2);
      i--;
      j--;
    }
  }
  return Cache[0][0];
}

// Recieve the Row and column from the children via pipes
void ReceiveRowAndColumn(int row_id, int col_id, int Cache[][len2 + 1], int * fd1, int * fd2) {
  int Matrix[len1 + 1][len2 + 1];
  close(fd1[1]);
  read(fd1[0], Cache[row_id], (len2 + 1) * 4);
  close(fd1[0]);
  close(fd2[1]);
  read(fd2[0], Matrix, (len1 + 1) * (len2 + 1) * 4);
  close(fd2[0]);
  copy_column(col_id, Matrix, Cache);
}

// Copy the 'col_id' from 'src[][]' to 'dest[][]'
void copy_column(int col_id, int src[][len2 + 1], int dest[][len2 + 1]) {
  for(int i = 0; i < len1; i++)
    dest[i][col_id] = src[i][col_id];
}

// Send the row with 'row_id' to the parent via pipes
void SendRow(int row_id, int Cache[][len2 + 1], int * fd) {
  close(fd[0]);
  write(fd[1], Cache[row_id], (len2 + 1) * 4);
  close(fd[1]);
}

// Send the column with 'col_id' to the parent via pipes
void SendColumn(int col_id, int Cache[][len2 + 1], int * fd) {
  close(fd[0]);
  write(fd[1], Cache, (len1 + 1) * (len2 + 1) * 4);
  close(fd[1]);
}

// Fills the Cell (i, j) appropriately
int FillCell(int x, int y, char * str1, char * str2, int Cache[][len2 + 1]) {
  if(str1[x] == str2[y])
    Cache[x][y] = 1 + Cache[x + 1][y + 1];
  else
    Cache[x][y] = maximum(Cache[x + 1][y], Cache[x][y + 1]);
  return Cache[x][y];
}

// Fills the row with 'row_id' appropriately
void FillRow(int row_id, int row_end, char * str1, char * str2, int Cache[][len2 + 1]) {
  for (int i = row_end; i >= 0; i--)
    FillCell(row_id, i, str1, str2, Cache);
}

// Fills the column with 'col_id' appropriately
void FillColumn(int col_id, int col_end, char * str1, char * str2, int Cache[][len2 + 1]) {
  for (int j = col_end; j >= 0; j--) 
    FillCell(j, col_id, str1, str2, Cache);
}

// Initialize the Cache table with base cases
void Initialize(int Cache[][len2 + 1]) {
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
