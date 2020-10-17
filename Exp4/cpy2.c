#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<fcntl.h>

int main() {
  int fd[2];
  pipe(fd);
  if (fork() == 0) {
    close(fd[0]);
    dup2(fd[1], 1);
    execlp("pwd", "pwd", NULL);
    printf("hello\n");
  }
  else {
    char buff[256];
    close(fd[1]);
    read(fd[0], buff, sizeof(buff));
    close(fd[0]);
    printf("Got string %s\n", buff);
  }
  return 0;
}
