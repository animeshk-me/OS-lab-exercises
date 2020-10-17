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

void CreateMagic(int Matrix[][size]);
void print_mat(int arr[][size] );

void CreateMagicEven(int Matrix[][size]);
void InitializeMatrix(int Matrix[][size]);
void ModifyQuarter(int Matrix[][size], int ptr, int start, int end);
void CopyMatrix(int Source[][size], int Dest[][size], int start_i, int start_j);

void CreateMagicOdd(int Matrix[][size]);
int wrap(int i);
void Wrapper(int *i, int *j);
void HorseMove(int *i, int *j); 
void NormalMove(int *i, int *j); 


int main() {
  printf("Kindly Enter either Odd Size or Doubly Even Size. Singly even order(4*n + 2 type) IS NOT ALLOWED.\n\nEnter the Size N of N X N Magic Square: ");
  scanf("%d", &size);
  int Matrix[size][size];
  memset(Matrix, 0, sizeof(Matrix));
  CreateMagic(Matrix);
  printf("The Magic Square row by row: \n");
  print_mat(Matrix);
  return 0;
}

// Creates the Magic Square
void CreateMagic(int Matrix[][size]) {
  if (size % 2 != 0)  // Odd Order Magic Square
    CreateMagicOdd(Matrix);
  else if (size % 4 == 0) // Doubly Even Order Magic Square
    CreateMagicEven(Matrix);
  else {
    printf("[X]Invalid Input: Singly Even Order Detected :(\nExiting...\n");
    exit(1);
  }
}

/************** The Doubly even order Magic Square Calculations ************/

// Creates the Magic Square of Doubly even Order
void CreateMagicEven(int Matrix[][size]) {
  InitializeMatrix(Matrix);
  pid_t pid1, pid2, pid3, pid4;
  int fd1[2], fd2[2], fd3[2], fd4[2];
  pipe(fd1);
  pipe(fd2);
  pipe(fd3);
  pipe(fd4);

  // Forking 4 child processes to establish parellelism
  ((pid1 = fork()) && (pid2 = fork()) && (pid3 = fork()) && (pid4 = fork()));
  
  if (pid1 == 0) {
    for (int i = 0; i < size / 4; i++)  // Modify Top left quarter square
      ModifyQuarter(Matrix, i, 0, size / 4);
    close(fd1[0]); 
    write(fd1[1], Matrix, size*size*4);  // Sending the modified Matrix to Parent
    close(fd1[1]);
    exit(0);
  }
  else if (pid2 == 0) {
    for (int i = 0; i < size / 4; i++)  // Modify Bottom Left quarter square
      ModifyQuarter(Matrix, i, 3 * size / 4, size);
    close(fd2[0]);
    write(fd2[1], Matrix, size*size*4); // Sending the modified Matrix to Parent
    close(fd2[1]);
    exit(0);
  }
  else if (pid3 == 0) {
    for (int i = 3 * size / 4; i < size; i++) // Modify Top Right quarter square
      ModifyQuarter(Matrix, i, 0, size / 4);
    close(fd3[0]);
    write(fd3[1], Matrix, size*size*4); // Sending the modified Matrix to Parent
    close(fd3[1]);
    exit(0);
  }
  else if (pid4 == 0) {
    for (int i = 3 * size / 4; i < size; i++) // Modify Bottom Right quarter square
      ModifyQuarter(Matrix, i, 3 * size / 4, size);
    close(fd4[0]);
    write(fd4[1], Matrix, size*size*4); // Sending the modified Matrix to Parent
    close(fd4[1]);
    exit(0);
  }
  else {
    int TopLeft[size][size], BottomLeft[size][size], TopRight[size][size], BottomRight[size][size];
    // Reading the Modified Quarter Square segments from the 4 children
    close(fd1[1]);
    read(fd1[0], TopLeft, size * size * 4);
    close(fd1[0]);

    close(fd2[1]);
    read(fd2[0], BottomLeft, size * size * 4);
    close(fd2[0]);

    close(fd3[1]);
    read(fd3[0], TopRight, size * size * 4);
    close(fd3[0]);

    close(fd4[1]);
    read(fd4[0], BottomRight, size * size * 4);
    close(fd4[0]);

    for (int i = size / 4; i < 3 * size / 4; i++) // Modify Central semi square
      ModifyQuarter(Matrix, i, size / 4, 3 * size / 4); 
    
    // Copying the modified Quarter Square Segments from children to the Main Matrix
    CopyMatrix(TopLeft, Matrix, 0, 0);
    CopyMatrix(TopRight, Matrix, 0, 3 * size / 4);
    CopyMatrix(BottomLeft, Matrix, 3 * size / 4, 0);
    CopyMatrix(BottomRight, Matrix, 3 * size / 4, 3 * size / 4);
  }
}

// Copies the passed portions of Dest[][] to Source[][]
void CopyMatrix(int Source[][size], int Dest[][size], int start_i, int start_j) {
  for (int i = start_i; i < start_i + (size / 4); i++) {
    for (int j = start_j; j < start_j + (size / 4); j++) 
      Dest[i][j] = Source[i][j];
  }
}

// Initializes the Doubly even order matrix
void InitializeMatrix(int Matrix[][size]) {
  int k = 1;
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      Matrix[i][j] = k;
      k++;
    }        
  }
}

// Modifies the matrix based upon the algorithm
void ModifyQuarter(int Matrix[][size], int ptr, int start, int end) {
  for(int i = start; i < end; i++)
    Matrix[i][ptr] = (size*size + 1) - Matrix[i][ptr];
}

/***************************************************************************/


/********************* The Odd order Magic Square Calculations **************/

// Creates the Magic Square of Doubly even Order
void CreateMagicOdd(int Matrix[][size]) {
  int max_num = size*size;
  int x = size / 2;
  int y = size - 1;
  int i = 1;
  Matrix[x][y] = i;
  i++;
  while(i <= max_num) {
    NormalMove(&x, &y);
    while(Matrix[x][y] != 0) // Do horsemoves until reach an empty cell  
      HorseMove(&x, &y);
    Matrix[x][y] = i;
    i++;
  }
}

// A normal move in case of next cell is not pre-occupied cell 
void NormalMove(int *i, int *j) {
  *i = *i - 1;
  *j = *j + 1;
  Wrapper(i, j);
}

// A horse move in case of already occupied next cell
void HorseMove(int *i, int *j) {
  *i = *i + 1;
  *j = *j - 2;
  Wrapper(i, j);
}

// A function to handle overflow and underflow of indices
void Wrapper(int *i, int *j) {
  if ((*i == -1) && (*j == size)) { // If you overflow both
    *i = 0;
    *j = size - 2;
    return;
  }
  *i = wrap(*i);
  *j = wrap(*j);
}

// A utility used to perform 'intelligent mod'
int wrap(int i) {
  int c = i % size;
  if (c < 0)
    c += size;
  return c;
}

/***************************************************************************/


// A simple utility to print out the matrix
void print_mat(int arr[][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      printf("%d  ", arr[i][j]);
    printf("\n");
  }
  printf("\n");
}
/***************************************************************************/