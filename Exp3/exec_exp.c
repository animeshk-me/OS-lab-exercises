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
    execl("/bin/ls", "ls", NULL);
  }
  printf("I'll come only once\n");
}