#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define SIZE 100

int len1 = 0;
int len2 = 0;

void FillRow(int row_id, int row_end, char * str1, char * str2, int Cache[][len2 + 1]);
void FillColumn(int col_id, int col_end, char * str1, char * str2, int Cache[][len2 + 1]);
void FillCell(int x, int y, char * str1, char * str2, int Cache[][len2 + 1]);
int LCS_Multi(char * str1, char * str2);

void Initialize(int Cache[][len2 + 1]);
void get_input(char * str, char * msg);
void RipOffNextLine(char * str);
int LCS(char * str1, char * str2, int idx1, int idx2);
int LCS_DP(char * str1, char * str2);
int maximum(int n1, int n2);


int main() {
  char str1[SIZE];
  char str2[SIZE];
  int indices[2];
  memset(str1, 0, SIZE);
  memset(str2, 0, SIZE);
  get_input(str1, "first");
  get_input(str2, "second");
  len1 = strlen(str1);
  len2 = strlen(str2);
  printf("%d\n", LCS_DP(str1, str2));
  return 0;
}

/********************** Simple DP codes *********************/

// Without DP
int LCS(char * str1, char * str2, int idx1, int idx2) {
  if (idx1 == strlen(str1) || (idx2 == strlen(str2)))
    return 0;
  else if (str1[idx1] == str2[idx2]) {
    // printf("good\n");
    return (1 + LCS(str1, str2, idx1 + 1, idx2 + 1));
  }
  else {
    int skip1 = LCS(str1, str2, idx1 + 1, idx2);
    int skip2 = LCS(str1, str2, idx1, idx2 + 1);
    return maximum(skip1, skip2);
  }
}

// With bottom-up DP
int LCS_DP(char * str1, char * str2) {
  int Cache[len1 + 1][len2 + 1];
  memset(Cache, -1, sizeof(Cache));
  Initialize(Cache);
  for(int i = len1 - 1; i >= 0; i--) {
    for(int j = len2 - 1; j >= 0; j--) {
      if(str1[i] == str2[j])
        Cache[i][j] = 1 + Cache[i + 1][j + 1];
      else
        Cache[i][j] = maximum(Cache[i + 1][j], Cache[i][j + 1]);
    }
  }
  return Cache[0][0];
}

/***************************************************************/

/**************** Multiprocessing Setup ************************/

// With multiprocessing
int LCS_Multi(char * str1, char * str2) {
  int Cache[len1 + 1][len2 + 1];
  pid_t pid1, pid2, wpid;
  int status = 0; 
  int fd1[2];
  int fd2[2];
  memset(Cache, -1, sizeof(Cache));
  Initialize(Cache);
  int i = len1 - 1;
  int j = len2 - 1;
  while((i >= 0) && (j >= 0)) {
    pipe(fd1);
    pipe(fd2);
    (pid1 = fork()) && (pid2 = fork());
    if (pid1 == 0) {      // First Child Process
      FillRow(i, j - 1, str1, str2, Cache);
      SendRow(i, fd1, Cache);
      exit(0);
    }
    else if (pid2 == 0) { // Second Child Process
      FillColumn(j, i - 1, str1, str2, Cache);
      SendColumn(j, fd2, Cache);
      exit(0);
    }
    else {          // Parent Process
      FillCell(i, j, str1, str2, Cache);
      while ((wpid = wait(&status)) > 0) // wait till all the children finish
      
      i--;
      j--;
    }
  }

  for(int i = len1 - 1; i >= 0; i--) {
    for(int j = len2 - 1; j >= 0; j--) {
      if(str1[i] == str2[j])
        Cache[i][j] = 1 + Cache[i + 1][j + 1];
      else
        Cache[i][j] = maximum(Cache[i + 1][j], Cache[i][j + 1]);
    }
  }
  return Cache[0][0];
}

// Send the row with 'row_id' to the parent via pipes
void SendRow(int row_id, int Cache[][len2 + 1], int * fd) {
  close(fd[0]);
  write(fd[1], Cache[row_id], len2 * 4);
  close(fd[1]);
}

// Send the column with 'col_id' to the parent via pipes
void SendColumn(int col_id, int Cache[][len2 + 1], int * fd) {
  close(fd[0]);
  write(fd[1], Cache[col_id], len1 * 4);
  close(fd[1]);
}

// Fills the Cell (i, j) appropriately
void FillCell(int x, int y, char * str1, char * str2, int Cache[][len2 + 1]) {
  if(str1[x] == str2[y])
    Cache[x][y] = 1 + Cache[x + 1][y + 1];
  else
    Cache[x][y] = maximum(Cache[x + 1][y], Cache[x][y + 1]);
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

// Take input string in 'str' and get indices of the substring
void get_input(char * str, char * msg) {
  printf("Enter the %s string: \n", msg);
  fgets(str, SIZE, stdin);
  RipOffNextLine(str);
}

// A simple utility to rip off the trailing '\n' from 'str'
void RipOffNextLine(char * str) {
  int i = 0;
  while(str[i] != '\n') 
    i++;
  str[i] = '\0';
}
