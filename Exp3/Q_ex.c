#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>


int Fib(int n);
int foo(int n);

int main() {
  int n;
  printf("Enter the number n: ");
  scanf("%d", &n);
  printf("The Fib(%d): %d\n", n, foo(n));
}


int Fib(int n) {
  if (n < 2) 
    return n;  
  int first, second;
  pid_t pid1, pid2, wpid;
  int status = 0;
  ((pid1 = fork()) && (pid2 = fork()));
  if (pid1 == 0) {
    first = Fib(n - 1);    
  } else if(pid2 == 0) {
    second = Fib(n - 2);
  } else {
    //parent block
    while ((wpid = wait(&status)) > 0); // wait till all the children finish
    printf("helo \n");
    return first + second;     
  }
}

int foo(int n) {
  pid_t pid = fork();
  int *p = &n;
  if (pid == 0)
    n = n * 2;
  else {
    wait();
    return n + 10;
  }
}