#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>

// A block to hold the data to passed among threads
struct block {
  int num;      // the number
  int pow_sum;  // the powered sum of the number NUM
};


int get_powered_sum(int n);
void *runner(void * params);
int Pow(int n, int x);

int main() {
  int a, b;
  printf("Enter a and b for the range [a, b] both inclusive: ");
  scanf("%d %d", &a, &b);

  pthread_t tid[b - a + 1];      // set of all TIDs
  struct block args[b - a + 1];  // set of all argument BLOCKSs
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  
  printf("The Armstrong numbers in the range [a, b]: \n");

  // Creating threads for all numbers
  for (int i = a; i <= b; i++) {
    args[i - a].num = i;
    pthread_create(&tid[i - a], &attr, runner, &args[i - a]);
  } 

  // Waiting for all threads to complete(pthread_join())
  for (int i = a; i <= b; i++) {
    pthread_join(tid[i - a], NULL);
  } 
  return 0;
}

// The thread routine to be run
void *runner(void * params) {
  struct block * args = params;
  args->pow_sum = get_powered_sum(args->num);
  if (args->num == args->pow_sum)
    printf("%d\n", args->num);
  pthread_exit(NULL);
}

// powered sum of digits of N
int get_powered_sum(int n) {
  int len = 0;
  int sum = 0;
  int i = n;
  while(i != 0) {
    i /= 10;
    len++;  // length calculation
  }
  i = n;
  while(i != 0) {
    sum += Pow((i % 10), len);  // pow_sum calculation
    i /= 10;
  }
  return sum;
}

// Power function (n^x)
int Pow(int n, int x) {
  if (x == 0)   return 1;
  int prod = n;
  while(x > 1) {
    prod *= n;
    x--;
  }
  return prod;
}