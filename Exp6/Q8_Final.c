#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

#define SINGULAR_MATRIX 0
#define INVERSE_EXISTS 1

// A package to pass data to get_inverse()
struct block_inv {
  int i;
  int j;
  float ** Matrix;
  float ** Inter;
  int size;
};

// A package to pass data to get_transpose()
struct block_trans {
  int i;
  int j;
  float ** src;
  float ** dest;
};

void * runner_trans(void * params);
void * runner_inv(void * params);
int get_inverse(float **Matrix, float **Inverse, int size);
void get_transpose(float ** dest, float ** src, int size);
float get_determinant(float ** Matrix, int size);
float get_cofactor(int x, int y, float ** Matrix, int size);
void PrintMat(float **Matrix, int size);
int sign(int x, int y);


int main() {
  int n;  // size of the matrix
  printf("Enter size n for the n x n square matrix: ");
  scanf("%d", &n);
  // Dynamic memory allocation
  float ** Matrix = malloc(sizeof(float *) * n);
  for (int i = 0; i < n; i++) {
    Matrix[i] = malloc(sizeof(float) * n);
    for (int j = 0; j < n; j++)
      scanf("%f", &Matrix[i][j]);
  }

  float ** Inverse = malloc(sizeof(float *) * n);
  for (int i = 0; i < n; i++)
    Inverse[i] = malloc(sizeof(float) * n);

  if(get_inverse(Matrix, Inverse, n) == SINGULAR_MATRIX) {
    fprintf(stderr, "The Inverse is impossible, since the Matrix is singular.\n");
  }
  else {
    printf("The Inverse of the input matrix is: \n");
    PrintMat(Inverse, n);
  }
  // Freeing the allocated memory
  for (int i = 0; i < n; i++) 
    free(Matrix[i]);
  free(Matrix);
  for (int i = 0; i < n; i++) 
    free(Inverse[i]);
  free(Inverse);
  return 0;
}


/******************** Inverse Creation ***********************/


// Returns the inverse(Matrix[][]) in Inverse[]
int get_inverse(float **Matrix, float **Inverse, int size) {
  float determinant = get_determinant(Matrix, size); 
  if (determinant == 0)
    return SINGULAR_MATRIX;

  float ** Intermediate;
  pthread_t tid[size*size];
  struct block_inv args[size*size];
  int ptr = 0;
  Intermediate = malloc(sizeof(float *) * size);
  for (int i = 0; i < size; i++) {
    Intermediate[i] = malloc(sizeof(float) * size);
    for (int j = 0; j < size; j++) {
      args[ptr].i = i;
      args[ptr].j = j;
      args[ptr].Inter = Intermediate;
      args[ptr].Matrix = Matrix;
      args[ptr].size = size;
      pthread_create(&tid[ptr], NULL, runner_inv, &args[ptr]);
      ptr++;
    }
  }
  // Adjoint matrix creation
  for(ptr = 0; ptr < size*size; ptr++)
    pthread_join(tid[ptr], NULL);
  get_transpose(Inverse, Intermediate, size); 
  
  for(int i = 0; i < size; i++) 
    free(Intermediate[i]);
  free(Intermediate);
  
  // Converting Adjoint to inverse
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      Inverse[i][j] = Inverse[i][j] / determinant;
  }
  return INVERSE_EXISTS;
}

// The thread routine to calculate cofactors to create Adjoint
void * runner_inv(void * params) {
  struct block_inv * args = params;
  args->Inter[args->i][args->j] = get_cofactor(args->i, args->j, args->Matrix, args->size);
  pthread_exit(NULL);  
}

/**************************************************************/


/******************** Transpose logic ***********************/


// Returns transpose of 'src' into 'dest'
void get_transpose(float ** dest, float ** src, int size) { 
  pthread_t tid[size*size];
  struct block_trans args[size*size];
  int ptr = 0;
  for (int i = 0; i < size; i++) { 
    for (int j = 0; j < size; j++) {
      args[ptr].i = i;
      args[ptr].j = j;
      args[ptr].src = src;
      args[ptr].dest = dest;
      pthread_create(&tid[ptr], NULL, runner_trans, &args[ptr]);
      ptr++;    
    }
  } 
  for(ptr = 0; ptr < size*size; ptr++)
    pthread_join(tid[ptr], NULL);
} 

// The thread routine to swap data to create transpose
void * runner_trans(void * params) {
  struct block_trans * args = params;
  int i = args->i;
  int j = args->j;
  args->dest[i][j] = args->src[j][i];
  pthread_exit(NULL);  
}

/**************************************************************/


/************ Determinant and Cofactor calculators ***********/

// Returns the determinant of square Matrix[][]
float get_determinant(float ** Matrix, int size) {
  if (size == 2) // Base Case
    return ((Matrix[0][0] * Matrix[1][1]) - (Matrix[0][1] * Matrix[1][0])); 
  float sum = 0;
  for(int j = 0; j < size; j++)
    sum += (Matrix[0][j] * get_cofactor(0, j, Matrix, size));
  return sum;
}

// Returns the cofactor(x, y) (the signed minor M(x, y))
float get_cofactor(int x, int y, float ** Matrix, int size) {
  float **Cofactor = malloc(sizeof(float *) * size);
  for (int i = 0; i < size; i++)
    Cofactor[i] = malloc(sizeof(float) * size);
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
  float determinant = sign(x, y) * get_determinant(Cofactor, size - 1);
  for (int i = 0; i < size; i++) 
    free(Cofactor[i]);
  free(Cofactor);
  return determinant;
}

/**************************************************************/


/******************** General functions ***********************/

// Returns the appropriate sign
int sign(int x, int y) {
  if ((x + y) % 2 == 0)
    return 1;
  return -1;
}

// Prints the Matrix[][]
void PrintMat(float **Matrix, int size) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%f  ", Matrix[i][j]);
    }
    printf("\n");
  }
}
/**************************************************************/