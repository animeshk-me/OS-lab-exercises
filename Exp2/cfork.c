#include<stdio.h>

int main() {
  // int pid = fork();
  // if (pid == 0) {
  //   printf("Child\n");
  //   int pid2 = fork();
  //   if (pid2 == 0)
  //     printf("Grand child\n");
  //   printf("I will come two times\n");
  // }
  // pid = fork();
  // if (pid == 0)
  //   printf("another child\n");
  // // fork();
  // printf(" hello \n");  

  for (int i = 0; i < 2; i++) {
    int pid = fork();
    if (pid == 0) printf("bad child  - %d", i);
    else printf("parent   %d\n", i);
  }
  return 0;
}