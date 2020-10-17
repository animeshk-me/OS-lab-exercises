#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main  () {
  __pid_t pid = fork();
  if (pid == 0) {
    printf("I am Child(PID: %d) of my original parent: %d\n", getpid(), getppid());
    sleep(20);
    printf("I am Child(PID: %d) of my current parent: %d\n", getpid(), getppid());
    printf("Child block slept 20 secs -> finishes\n");
    exit(0);
  }
  else {
    sleep(5);
    printf("Parent block(PID: %d) slept 5 secs -> finishes and The child PID: %d is still running\n", getpid(), pid);
    return 0;
  }
}