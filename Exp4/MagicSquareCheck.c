#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define LEFT 0
#define RIGHT 1

int size = 0;

void take_input(int arr[][size]);
void print_mat(int arr[][size] );
int RowSumHandler(int Matrix[][size]);
int ColumnSumHandler(int Matrix[][size]);
int get_column_sum(int Matrix[][size], int ptr);
int get_row_sum(int Matrix[][size], int ptr);
int DiagonalSum(int flag, int Matrix[][size]);
int is_magic(int Matrix[][size]);

int main() {
  printf("Enter the Size N of N X N Square: ");
  scanf("%d", &size);
  int Matrix[size][size];
  printf("Enter the Matrix row by row: \n");
  take_input(Matrix);
  
  int magic_const = is_magic(Matrix);
  if(magic_const > 0)
    printf(":) The Matrix IS a Magic Square with Magic Constant %d \n", magic_const);
  else
    printf("The Matrix IS NOT a Magic Square :( \n");
  return 0;
}

// This function calculates the magic constant of the square(If it is Magic Sq)
int is_magic(int Matrix[][size]) {
  pid_t pid1, pid2, pid3, pid4;
  int fd1[2], fd2[2], fd3[2], fd4[2];
  pipe(fd1);
  pipe(fd2);
  pipe(fd3);
  pipe(fd4);
  // Forking 4 child processes
  ((pid1 = fork()) && (pid2 = fork()) && (pid3 = fork()) && (pid4 = fork()));
  if (pid1 == 0)  {    // Child 1 block
    int sum = RowSumHandler(Matrix);
    // Send the 'sum' to parent process via pipe
    close(fd1[0]); 
    write(fd1[1], &sum, 4); 
    close(fd1[1]); 
    // Succesful termination
    exit(0);
  }
  else if (pid2 == 0)  { // Child 2 block
    int sum = ColumnSumHandler(Matrix);
    // Send the 'sum' to parent process via pipe
    close(fd2[0]); 
    write(fd2[1], &sum, 4); 
    close(fd2[1]); 
    // Succesful termination
    exit(0);
  }
  else if (pid3 == 0)  {  // Child 3 block
    int sum = DiagonalSum(LEFT, Matrix);
    // Send the 'sum' to parent process via pipe
    close(fd3[0]); 
    write(fd3[1], &sum, 4); 
    close(fd3[1]); 
    // Succesful termination
    exit(0);
  }
  else if (pid4 == 0)  {   // Child 4 block
    int sum = DiagonalSum(RIGHT, Matrix);
    // Send the 'sum' to parent process via pipe
    close(fd4[0]); 
    write(fd4[1], &sum, 4); 
    close(fd4[1]); 
    // Succesful termination
    exit(0);
  }
  else {   // The Paren Block
    int row_sum, column_sum, diagonal_1_sum, diagonal_2_sum;
  
    // Recieving the Row Sum from First Child
    close(fd1[1]);
    read(fd1[0], &row_sum, 4);
    close(fd1[0]);

    // Recieving the Column Sum from Second Child
    close(fd2[1]);
    read(fd2[0], &column_sum, 4);
    close(fd2[0]);

    // Recieving the Sum of Main diagonal(LEFT) from Third Child
    close(fd3[1]);
    read(fd3[0], &diagonal_1_sum, 4);
    close(fd3[0]);

    // Recieving the Sum of Other diagonal(RIGHT)from Fourth Child
    close(fd4[1]);
    read(fd4[0], &diagonal_2_sum, 4);
    close(fd4[0]);    

    // Comparing the Sum values
    if ((row_sum == column_sum) && (column_sum == diagonal_1_sum) && (diagonal_1_sum == diagonal_2_sum)) 
      return row_sum; // Returning the Magic Constant of this Magic Square
    return -1;  // Returning the signal of not being a magic square
  }
}

// This function calculates the Diagonal Sum(RIGHT or LEFT) based on the flag value
int DiagonalSum(int flag, int Matrix[][size]) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    if (flag == LEFT)  // In case of main diagonal
      sum += Matrix[i][i];
    else               // In case of other diagonal
      sum += Matrix[size - 1 - i][i];
  }
  return sum;
}

// This function compares the sum of all the rows and returns final row_sum(if any)
int RowSumHandler(int Matrix[][size]) {
  int prev_sum = 0;
  int sum;
  for(int i = 0; i < size; i++) {
    int fd[2];
    pipe(fd);
    pid_t pid = fork(); // Forking the Child process to calculate the Row Sum
    if (pid == 0) {
      sum = get_row_sum(Matrix, i); // Calculates Row sum of ith row of the Matrix
      // Sending the row sum to the parent process
      close(fd[0]); 
      write(fd[1], &sum, 4); 
      close(fd[1]);
      exit(0); // successful termination
    }
    else {
      // Receiving the Row Sum from child process
      close(fd[1]); 
      read(fd[0], &sum, 4); 
      close(fd[0]); 
      if ((prev_sum != 0) && (prev_sum != sum)) // When the consecutive sums don't match
        return -1;  
      prev_sum = sum;
    }
  }
  return prev_sum; 
}

// This function compares the sum of all the columns and returns final column_sum(if any)
int ColumnSumHandler(int Matrix[][size]) {
  int prev_sum = 0;
  int sum;
  for(int i = 0; i < size; i++) {
    int fd[2];
    pipe(fd);
    pid_t pid = fork(); // Forking the Child process to calculate the Column Sum
    if (pid == 0) {
      sum = get_column_sum(Matrix, i); // Calculates Column sum of ith Column of the Matrix
      // Sending the column sum to the parent process
      close(fd[0]); 
      write(fd[1], &sum, 4); 
      close(fd[1]); 
      exit(0); // successful termination
    }
    else {
      // Receiving the Column Sum from child process
      close(fd[1]); 
      read(fd[0], &sum, 4); 
      close(fd[0]); 
      if ((prev_sum != 0) && (prev_sum != sum)) // When the consecutive sums don't match
        return -1;
      prev_sum = sum;
    }
  }
  return prev_sum;
}

// Returns the sum of ptr'th row in the Matrix[][size]
int get_row_sum(int Matrix[][size], int ptr) {
  int sum = 0;
  for (int i = 0; i < size; i++)
    sum += Matrix[ptr][i];
  return sum; 
}

// Returns the sum of ptr'th column in the Matrix[][size]
int get_column_sum(int Matrix[][size], int ptr) {
  int sum = 0;
  for (int i = 0; i < size; i++)
    sum += Matrix[i][ptr];
  return sum; 
}

// A simple utility to take the Matrix as input
void take_input(int arr[][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      scanf("%d", &arr[i][j]);
  }
}

// A simple utility to print out the matrix
void print_mat(int arr[][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      printf("%d ", arr[i][j]);
    printf("\n");
  }
  printf("\n");
}