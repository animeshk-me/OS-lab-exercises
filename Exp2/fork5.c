#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main() {
  pid_t pid_2, pid_3;
  
  (pid_2 = fork()) && (pid_3 = fork());

  if (pid_2 == 0) {
    pid_t pid_4, pid_5, pid_6;
    (pid_4 = fork()) && (pid_5 = fork());
    if (pid_4 == 0) {
      pid_t pid_8;
      pid_8 = fork();
      if (pid_8 == 0) {
        printf("I am process 8 block\n");
      } 
      else {
        printf("I am process 4 block\n");
      }
    }
    else if (pid_5 == 0) {
      pid_t pid_9;
      pid_9 = fork();
      if (pid_9 == 0) {
        printf("I am process 9 block\n");
      } 
      else {
        printf("I am process 5 block\n");
      }
    }
    else {
      pid_6 = fork();
      if (pid_6 == 0) {
        printf("I am process 6 block\n");
      }
      else {
        printf("I am process 2 block\n");
      }
    }
  } 
  else if (pid_3 == 0) {
    pid_t pid_7;
    pid_7 = fork();
    if (pid_7 == 0) {
      printf("I am process 7 block\n");
    }
    else {
      printf("I am process 3 block\n");
    }
  } 
  else {
    printf("I am process 1 block\n");
  }
}