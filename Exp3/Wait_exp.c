#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main() {
  pid_t pid1, pid2;
  (pid1 = fork()) && (pid2 = fork());
  if (pid1 == 0) {
    printf("First Child block\n");
  }
  else if (pid2 == 0) {
    printf("Second Child block\n");
  }
  else {
    int status = 0;
    printf("I(parent) wait for process 1\n");
    waitpid(pid1, &status, 0);
    printf("Status of child 1: %d\n", WIFEXITED(status));
    printf("I(parent) wait for process 2\n");
    waitpid(pid2, &status, 0);
    printf("Status of child 2: %d\n", WIFEXITED(status));
    printf("I am parent block\n");
  }
  return 0;
}