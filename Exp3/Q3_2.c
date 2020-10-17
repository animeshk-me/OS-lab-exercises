#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
 

int Pow(int n, int x);

int main() {
  int a, b;
  printf("Enter a and b for the range [a, b] both inclusive: ");
  scanf("%d %d", &a, &b);
  printf("The Armstrong numbers in the range [a, b]: \n");
  for (int n = a; n <= b; n++) {
    int sum = 0;
    int i = n;
    int fd[2]; 
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {      // Child
      int len = 0;
      while(i != 0) {
        i /= 10;
        len++;
      }
      i = n;
      while(i != 0) {
        sum += Pow((i % 10), len);
        i /= 10;
      }
      close(fd[0]); 
      write(fd[1], &sum, 100); 
      close(fd[1]); 
      exit(0);
    }
    else {              // parent
      int status = 0;
      waitpid(pid, &status, 0);
      close(fd[1]);  
      read(fd[0], &sum, 100); 
      close(fd[0]); 
      if (sum == n)
        printf("%d \n", n);
    }
  } 
  return 0;
}

int Pow(int n, int x) {
  if (x == 0)   return 1;
  int prod = n;
  while(x > 1) {
    prod *= n;
    x--;
  }
  return prod;
}