#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

void take_input(int arr[][3]);
void print_mat(int arr[][3]);
void Multiply(int Product[][3], int First[][3], int Second[][3], int size);
int Component(int ptr1, int ptr2, int First[][3], int Second[][3], int size);

int main() {
  int First[3][3];
  int Second[3][3];
  int Product[3][3];
  printf("Enter the First matrix row by row: \n");
  take_input(First);
  printf("Enter the Second matrix row by row: \n");
  take_input(Second);
  Multiply(Product, First, Second, 3);
  printf("The Product matrix is as below: \n");
  print_mat(Product);
  return 0;
}

// A Multiprocessing implementation of Matrix Multiplication Algorithm
void Multiply(int Product[][3], int First[][3], int Second[][3], int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      int fd[2];          // File Descriptor Array
      pipe(fd);
      if (fork() == 0) {  // Child Process
        int prod = Component(i, j, First, Second, size);
        
        // Writing the Component Product value to the pipe
        close(fd[0]); 
        write(fd[1], &prod, 10); 
        close(fd[1]); 
        exit(0);
      } 
      else {             // Parent Process
        // Reading the Component Product value from the pipe
        close(fd[1]); 
        read(fd[0], &Product[i][j], 10); // Updating the Product matrix
        close(fd[0]); 
      } 
    }
  }
}

// Component Product Calculator
int Component(int ptr1, int ptr2, int First[][3], int Second[][3], int size) {
  int sum = 0;
  for (int i = 0; i < size; i++)
    sum += (First[ptr1][i] * Second[i][ptr2]);
  return sum;
}

// A simple utility to take the Matrix as input
void take_input(int arr[][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      scanf("%d", &arr[i][j]);
  }
}

// A simple utility to print out the matrix
void print_mat(int arr[][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++)
      printf("%d ", arr[i][j]);
    printf("\n");
  }
  printf("\n");
}