#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int Fib(int n);
int sum = 0;

int main() {
  int n;
  printf("Enter the number n: ");
  scanf("%d", &n);
  printf("Fibonacci series upto nth fibonacci number: \n");
  for(int i = 0; i <= n; i++) 
    printf("Fib(%d): %d\n", i, Fib(i));
  return 0;
}


int Fib(int n) {
  if (n < 2) 
    return n;  
  int first, second;
  pid_t pid1, pid2, wpid;
  int status = 0;
  int fd1[2], fd2[2]; 
  pipe(fd1);
  pipe(fd2);
  ((pid1 = fork()) && (pid2 = fork()));
  if (pid1 == 0) { // Child 1
    first = Fib(n - 1);
    close(fd1[0]); 
    write(fd1[1], &first, 100); 
    close(fd1[1]); 
    exit(0);
  } 
  else if(pid2 == 0) { // child 2
    second = Fib(n - 2);
    close(fd2[0]); 
    write(fd2[1], &second, 100); 
    close(fd2[1]); 
    exit(0);
  } 
  else {
    //parent block
    while ((wpid = wait(&status)) > 0); // wait till all the children finish
    close(fd1[1]);  
    read(fd1[0], &first, 100); 
    sum = first;
    close(fd1[0]); 
    close(fd2[1]);    
    read(fd2[0], &second, 100); 
    close(fd2[0]); 
    return sum + second;     
  }
}