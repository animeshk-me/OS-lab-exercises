#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

int len1, len2;

void Initialize(int Cache[][len2 + 1]);
int LCS(char * str1, char * str2);
void FillRow(int row_id, int row_end, char * str1, char * str2, int Cache[][len2 + 1]);
void FillColumn(int col_id, int col_end, char * str1, char * str2, int Cache[][len2 + 1]);
int FillCell(int x, int y, char * str1, char * str2, int Cache[][len2 + 1]);
void SendRow(int row_id, int Cache[][len2 + 1], int * fd);
void SendColumn(int col_id, int Cache[][len2 + 1], int * fd);
void copy_column(int col_id, int src[][len2 + 1], int dest[][len2 + 1]);
void ReceiveRowAndColumn(int row_id, int col_id, int Cache[][len2 + 1], int * fd1, int * fd2);

int maximum(int n1, int n2);
void get_input(char * str, char * msg);
void RipOffNextLine(char * str);