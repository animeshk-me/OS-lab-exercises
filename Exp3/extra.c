#include<stdio.h>
#include<unistd.h>

int main() {
  printf("I am another process, called by %d \n", getppid());
  return 0;
}