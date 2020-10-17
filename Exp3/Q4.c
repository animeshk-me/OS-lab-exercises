#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>


int is_prime(int n);

int main() {
  int n;
  printf("Enter the number n: ");
  scanf("%d", &n);
  pid_t pid = fork();
  if (pid == 0) {
    // Child block
    printf("All the prime numbers smaller than or equal to n: ");
    for (int i = 2; i <= n; i++) { 
      if (is_prime(i)) 
        printf("%d ", i); 
    } 
    printf("\n");
  } else {
    // Parent block
    printf("Fibonacci series upto nth fibonacci number: ");
    int prev, prev_prev;
    if (n >= 1)
      printf("0 ");
    if (n >= 2)
      printf("1 ");
    prev_prev = 0;
    prev = 1;
    for (int i = 0; i <= n; i++) {
      int temp = prev;
      prev = prev_prev + prev;
      prev_prev = temp;
      printf("%d ", prev);
    }
    printf("\n");
  }
}

int is_prime(int n) { 
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