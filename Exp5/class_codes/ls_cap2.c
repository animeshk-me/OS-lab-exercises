#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>
#include <ctype.h> 

int main () {
  char *cmd1[] = { "/bin/ls", "-al", "/", 0 };
  char *cmd2[] = { "/usr/bin/tr", "a-z", "A-Z", 0 };
  int pid; int pfd[2];
  pipe (pfd); // other validation code u can repeat from earlier discussion
  switch (pid = fork()) { 
    case 0: /* child */
      dup2(pfd[0], 0);
      close(pfd[1]); /* the child does not need this end of the pipe */
      execvp(cmd2[0], cmd2); // executes tr command on output of ls stored at the respective pipe end
      perror(cmd2[0]);

    default: /* parent */
      dup2(pfd[1], 1);
      close(pfd[0]);
      /* the parent does not need this end of the pipe */
      execvp(cmd1[0], cmd1);
      perror(cmd1[0]);
      // ls executed and written to the pipe end of 1
    case -1:
      perror("fork");
      exit(1);
  }
  return 0;
}
