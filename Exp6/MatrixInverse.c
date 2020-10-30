#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

#define MAX_SIZE 100
#define SINGULAR_MATRIX 0
#define INVERSE_EXISTS 1

int get_inverse(float Matrix[][MAX_SIZE], float Inverse[][MAX_SIZE], int size);
void get_transpose(float dest[][MAX_SIZE], float src[][MAX_SIZE], int size);
float get_determinant(float Matrix[][MAX_SIZE], int size);
float get_cofactor(int x, int y, float Matrix[][MAX_SIZE], int size);
void PrintMat(float Matrix[][MAX_SIZE], int size);
int sign(int x, int y);

int main() {
  int n;
  float Matrix[MAX_SIZE][MAX_SIZE];
  float Inverse[MAX_SIZE][MAX_SIZE];
  printf("Enter size n for the n x n square matrix: ");
  scanf("%d", &n);
  for(int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) 
      scanf("%f", &Matrix[i][j]);
  }
  if(get_inverse(Matrix, Inverse, n) == SINGULAR_MATRIX) {
    fprintf(stderr, "The Inverse is impossible, since the Matrix is singular.\n");
  }
  else {
    printf("The Inverse of the input matrix is: \n");
    PrintMat(Inverse, n);
  }
  return 0;
}

// Returns the inverse(Matrix[][]) in Inverse[]
int get_inverse(float Matrix[][MAX_SIZE], float Inverse[][MAX_SIZE], int size) {
  float Intermediate[MAX_SIZE][MAX_SIZE];
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      Intermediate[i][j] = get_cofactor(i, j, Matrix, size);
  }
  get_transpose(Inverse, Intermediate, size); // Adjoint matrix
  float determinant = get_determinant(Matrix, size); 
  if (determinant == 0)
    return SINGULAR_MATRIX;
  
  // Converting Adjoint to inverse
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      Inverse[i][j] = Inverse[i][j] / determinant;
  }
  return INVERSE_EXISTS;
}

// Returns transpose of 'src' into 'dest'
void get_transpose(float dest[][MAX_SIZE], float src[][MAX_SIZE], int size) { 
  for (int i = 0; i < size; i++){ 
    for (int j = 0; j < size; j++) 
      dest[i][j] = src[j][i];
  } 
} 

// Returns the determinant of square Matrix[][]
float get_determinant(float Matrix[][MAX_SIZE], int size) {
  if (size == 2)
    return ((Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0])); 
  float sum = 0;
  for(int j = 0; j < size; j++)
    sum += (Matrix[0][j] * get_cofactor(0, j, Matrix, size));
  return sum;
}

// Returns the cofactor(x, y) (the signed minor M(x, y))
float get_cofactor(int x, int y, float Matrix[][MAX_SIZE], int size) {
  float Cofactor[MAX_SIZE][MAX_SIZE];
  int cof_i = 0;
  int cof_j = 0;
  for (int j = 0; j < size; j++) {
    if (j != y) {
      cof_i = 0;
      for (int i = 0; i < size; i++) {
        if(i != x) {
          Cofactor[cof_i][cof_j] = Matrix[i][j];
          cof_i++;
        }
      }
      cof_j++;
    }
  }
  return (sign(x, y) * get_determinant(Cofactor, size - 1));
}

// Returns the appropriate sign
int sign(int x, int y) {
  if ((x + y) % 2 == 0)
    return 1;
  return -1;
}

// Prints the Matrix[][]
void PrintMat(float Matrix[][MAX_SIZE], int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%f  ", Matrix[i][j]);
    }
    printf("\n");
  }
}
