#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>


// A block helds the number with its fibonacci number
struct block {
  int num; // the number
  int fib_num; // finbonacci of 'num'
};

int Fib(int n);
void* runner(void *params);

int main() {
  int n;
  printf("Enter the number n: ");
  scanf("%d", &n);
  printf("Fibonacci series upto nth fibonacci number: \n");
  for(int i = 0; i <= n; i++) 
    printf("Fib(%d): %d\n", i, Fib(i));
  return 0;
}

// Generates nth fibonacci number
int Fib(int n) {
  if (n < 2) 
    return n;  
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  struct block number1, number2;
  number1.num = n - 1;
  number2.num = n - 2;
  pthread_attr_init(&attr);

  // Calculate fib(n-1) and fib(n-2)
  pthread_create(&tid1, &attr, runner, &number1);
  pthread_create(&tid2, &attr, runner, &number2);
  pthread_join(tid1, NULL); // Wait for tid1 thread to complete
  pthread_join(tid2, NULL); // Wait for tid2 thread to complete
  return (number1.fib_num + number2.fib_num); // return the sum
}

// The Thread routine executed by Fib(n)
void* runner(void *params) {
  struct block * b = params;
  b->fib_num = Fib(b->num);
  pthread_exit(NULL);
}
