#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>
#include <ctype.h> 

#define MAX 512

int main(int argc, char* argv[]) { 
  int fd[2]; char buf[MAX]; int nb, i;
  if (pipe(fd) == -1) { 
    perror("Creating pipe"); 
    exit(1); 
  }
  switch(fork()) { 
    case -1:
      perror("Creating a process ");
      exit(1);
    case 0:
      dup2(fd[1], 1);
      execvp("ls", argv); // output of ls command written on fd[1] courtsey the dup2 call.
      perror("program ls");
      exit(1);
    default:
      close(fd[1]);
      while ((nb=read(fd[0], buf, MAX)) > 0) {
        for(i=0; i<nb; i++)
          buf[i] = toupper(buf[i]);
        printf("Test %s \n",buf);
        if (write(1, buf, nb) == -1) { 
          perror ("Writting to stdout"); 
          exit(1); 
        }
      } // end of while
      if (nb == -1) { 
        perror("Reading from pipe");
        exit(1); 
      }
  } // end of switch
} // main end