#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define SIZE 10
// A package to pass around data among threads
struct block {
  int * arr;  // the array
  int start;  // the start index     
  int end;    // the end index
};

_Bool BelongsToCircle(float x, float y);
void Simulate();

int main() {
  int a = 9;
  Simulate();
  printf("\n");
  return 0;
} 

void Simulate() {
  int ScoreCircle = 0;
  int ScoreSquare = 0;
  srand(time(0));
  while(1) {
    float x = ((float)rand()/(float)(RAND_MAX)) * SIZE;
    float y = ((float)rand()/(float)(RAND_MAX)) * SIZE;
    if (BelongsToCircle(x, y)) 
      ScoreCircle++;
    ScoreSquare++;
    printf("PI Value: %f, S: %d, C: %d\n", ((float)(ScoreCircle * 4)) / ((float)(ScoreSquare)), ScoreSquare, ScoreCircle);
  }
}

// Returns 1 if (x, y) belongs to the circle
_Bool BelongsToCircle(float x, float y) {
  float radius = x * x + y * y;
  if (radius <= SIZE * SIZE)
    return 1;  // Belongs to circle
  return 0; // Belongs to square
}
