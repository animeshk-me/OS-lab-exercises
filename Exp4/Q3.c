#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  pid_t pid;
  int i = 1;
  while(1) {
    pid = fork();
    if(pid < 0) {
      printf("MAX no of concurrent process are %d\n",i);
      exit(0);
    }
    if(pid == 0) {
      printf("%d ", i);
      i++;
    }
    else {
      wait(0);
      exit(0);
    }
  }
  return 0;
}