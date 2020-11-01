#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define MAIN_DIAGONAL 0
#define TRAILING_DIAGONAL 1
#define NOT_A_MAGIC_SQUARE -1

int size = 0;

void take_input(int arr[][size]);
void print_mat(int arr[][size] );
int RowSumHandler(int Matrix[][size]);
int ColumnSumHandler(int Matrix[][size]);
int get_column_sum(int Matrix[][size], int ptr);
int get_row_sum(int Matrix[][size], int ptr);
int DiagonalSum(int diagonal_type, int Matrix[][size]);
int is_magic(int Matrix[][size]);

int main() {
  printf("Enter the Size N of N X N Square: ");
  scanf("%d", &size);
  int Matrix[size][size];
  printf("Enter the Matrix row by row: \n");
  take_input(Matrix);
  
  int magic_const = is_magic(Matrix);
  if(magic_const == NOT_A_MAGIC_SQUARE)
    printf(":( The Matrix IS NOT a Magic Square\n");
  else
    printf(":) The Matrix IS a Magic Square with Magic Constant %d \n", magic_const);
  return 0;
}

// This function calculates the magic constant of the square(If it is Magic Sq)
int is_magic(int Matrix[][size]) {
  pid_t pid1, pid2;
  int fd1[2], fd2[2];
  pipe(fd1);
  pipe(fd2);
  // Forking 2 child processes
  ((pid1 = fork()) && (pid2 = fork()));
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
  else {   // The Parent Block
    int row_sum, column_sum;
    int diagonal_1_sum = DiagonalSum(MAIN_DIAGONAL, Matrix);
    int diagonal_2_sum = DiagonalSum(TRAILING_DIAGONAL, Matrix);

    // Recieving the Row Sum from First Child
    close(fd1[1]);
    read(fd1[0], &row_sum, 4);
    close(fd1[0]);

    // Recieving the Column Sum from Second Child
    close(fd2[1]);
    read(fd2[0], &column_sum, 4);
    close(fd2[0]);  

    // Comparing the Sum values
    if ((row_sum == column_sum) && (column_sum == diagonal_1_sum) && (diagonal_1_sum == diagonal_2_sum)) 
      return row_sum; // Returning the Magic Constant of this Magic Square
    return NOT_A_MAGIC_SQUARE;  // Returning the signal of not being a magic square
  }
}

// This function calculates the Diagonal Sum(TRAILING_DIAGONAL or MAIN_DIAGONAL) based on the 'diagonal_type' value
int DiagonalSum(int diagonal_type, int Matrix[][size]) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    if (diagonal_type == MAIN_DIAGONAL)  // In case of main diagonal
      sum += Matrix[i][i];
    else               // In case of trailing diagonal
      sum += Matrix[size - 1 - i][i];
  }
  return sum;
}

// This function compares the sum of all the rows and returns final row_sum(if any)
int RowSumHandler(int Matrix[][size]) {
  int prev_sum = 0;
  int sum;
  for(int i = 0; i < size; i++) {
    sum = get_row_sum(Matrix, i); // Calculates Row sum of ith row of the Matrix
    if ((prev_sum != 0) && (prev_sum != sum)) // When the consecutive sums don't match
      return NOT_A_MAGIC_SQUARE;  
    prev_sum = sum;
  }
  return prev_sum; 
}

// This function compares the sum of all the columns and returns final column_sum(if any)
int ColumnSumHandler(int Matrix[][size]) {
  int prev_sum = 0;
  int sum;
  for(int i = 0; i < size; i++) { 
    sum = get_column_sum(Matrix, i); // Calculates Column sum of ith Column of the Matrix
    if ((prev_sum != 0) && (prev_sum != sum)) // When the consecutive sums don't match
      return NOT_A_MAGIC_SQUARE;
    prev_sum = sum;
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