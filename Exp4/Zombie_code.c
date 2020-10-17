#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main  () {
  __pid_t pid = fork();
  if (pid == 0) {
    printf("Child block finishes\n");
    exit(0);
  }
  else {
    sleep(50);
    printf("Parent block finishes\n");
    return 0;
  }
}