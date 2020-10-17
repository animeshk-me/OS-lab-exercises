#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main() {
  pid_t pid;
  pid = fork();
  if (pid == 0) {
    execl("./a.out", "a.out", NULL);
  } else {
    printf("Parent block \n");
    execlp("pwd", "pwd", NULL);
    printf("I will be left out\n");
  }
  printf("I'll come only once\n");
}