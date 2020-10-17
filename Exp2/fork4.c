#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main() {
  printf("A");
  fork();
  printf("B");
  return 0;
}