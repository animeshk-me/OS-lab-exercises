#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
 
int Pow(int n, int x);
void desc_sort(int * arr, int size);
void asc_sort(int * arr, int size);
void print(int * arr, int size);
void my_swap(int* x, int* y);

int main() {
  int n = 22334;
  int p[] = {0, 7, 4, 5};
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    printf("Child Block*****************\n");
    p[3] = 45;
    printf("%d ", p[3]);
  }
  else {           // Parent block
    printf("Parent Block*****************\n");
    printf("%d ", p[3]);
  }
  return 0;
}

