#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

#define SIZE 10
#define NUMTHREADS 10000

// A package to pass around data among threads
struct block {
  int * score_circle; // pointer to ScoreCircle 
  int * score_square; // pointer to ScoreSquare
};

_Bool BelongsToCircle(float x, float y);
void * runner(void * params);

pthread_mutex_t lock;  // A mutex object

int main() {
  int ScoreCircle = 0; // # points in Circle
  int ScoreSquare = 0; // # points in Square
  pthread_mutex_init(&lock, NULL); // Initialise mutex object
  pthread_t tid[NUMTHREADS];
  struct block args;
  args.score_circle = &ScoreCircle;
  args.score_square = &ScoreSquare;
  srand(time(0));
  
  // Creating NUMTHREADS which land NUMTHREADS points
  for (int i = 0; i < NUMTHREADS; i++)
    pthread_create(&tid[i], NULL, runner, &args);
  for (int i = 0; i < NUMTHREADS; i++) 
    pthread_join(tid[i], NULL);
  pthread_mutex_destroy(&lock); // Destroy the mutex object
  printf("\n");
  return 0;
} 

// The thread routine which lands one point on the SIZE X SIZE square
void * runner(void * params) {
  struct block * args = params;
  // Entering into the critical section
  pthread_mutex_lock(&lock);  // lock the mutex
  
  float x = ((float)rand()/(float)(RAND_MAX)) * SIZE;
  float y = ((float)rand()/(float)(RAND_MAX)) * SIZE;
  if (BelongsToCircle(x, y)) 
    *(args->score_circle) += 1;
  *(args->score_square) += 1;
  float PI = ((float)(*(args->score_circle) * 4)) / ((float)(*(args->score_square)));
  printf("PI Value: %f, S: %d, C: %d\n", PI, *args->score_square, *args->score_circle);

  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}

// Returns 1 if (x, y) belongs to the circle
_Bool BelongsToCircle(float x, float y) {
  float radius = x * x + y * y;
  if (radius <= SIZE * SIZE)
    return 1;  // Belongs to circle
  return 0; // Belongs to square
}


