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
    char *cmd[] = { "ls", "-ahl", (char *)0 };
    char *env[] = { "HOME=/usr/home", "LOGNAME=home", NULL };
    execve ("/bin/ls", cmd, env);
    printf("I will be left out\n");
  }
  printf("I'll come only once\n");
}