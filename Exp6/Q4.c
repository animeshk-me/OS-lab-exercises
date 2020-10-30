#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

// A package to pas around data among threads
struct block {
  int n;  // the number to be passed
};

_Bool is_prime(int n);
void * runner(void * params);

int main(int argc, char** argv) {
  int n = atoi(argv[1]);
  printf("The Prime numbers smaller than or equal to %d: \n", n);

  // Creating 1 thread for each number <= n
  pthread_t tid[n + 1];
  struct block args[n + 1];
  for (int i = 2; i <= n; i++) { 
    args[i].n = i;
    if(pthread_create(&tid[i], NULL, runner, &args[i]) != 0)
      fprintf(stderr, "Thread Creation failed...\nExiting...\n"); 
  } 
  // joining those threads
  for (int i = 2; i <= n; i++) { 
    pthread_join(tid[i], NULL); 
  } 
  printf("\n");
}

// The thread routine which prints the number if it is prime
void * runner(void * params) {
  struct block * args = params;
  if(is_prime(args->n)) {
    printf("%d ", args->n);
    fflush(stdout);
  }
  pthread_exit(NULL);
}

// Returns 1 if 'n' is a prime number else returns 0
_Bool is_prime(int n) { 
  if (n <= 1)
    return 0; 
  if (n <= 3) 
    return 1; 
  if ((n % 2 == 0) || (n % 3 == 0))
    return 0;
  for (int i = 5; i * i <= n; i = i + 6) { 
    if (n % i == 0 || n % (i + 2) == 0) 
      return 0;
  }
  return 1; 
} 
