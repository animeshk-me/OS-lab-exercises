#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main() {
  pid_t pid1, pid2;
  int status = 0;
  pid1 = fork();
  if (pid1 == 0) {
    printf("pid1 = %d waits for ppid = %d\n", getpid(), getppid());
    waitpid(getppid(), &status, 0);
    printf("pid1 ends\n");
  }
  else {
    printf("main = %d waits for pid1 = %d\n", getpid(), pid1);
    waitpid(pid1, &status, 0);
    printf("parent ends\n");
  }
}