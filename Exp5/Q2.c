/* Q1 and Q4 both are done here. Reversing the string in child. 
And checking the palindrome in parent. */

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define SIZE 100


void get_input(char * str, char * msg);
void RipOffNextLine(char * str);
void FreeChild(int * fd);
int WaitParent(int * fd);

int main() {
  char str[SIZE];
  memset(str, 0, SIZE);
  int fd1[2];  
  int fd2[2]; 
  int fd3[2];  // For Synchronizing the input from stdin by the two processes.
  pipe(fd1);
  pipe(fd2);
  pipe(fd3);
  pid_t pid = fork();
  if (pid == 0) {  // Child Process
    // Keep waiting until parent signals
    while(WaitParent(fd3)) { }

    char concat_str[SIZE];
    memset(concat_str, 0, SIZE);
    get_input(str, "the string to be concatenated"); 

    // Send the string to parent
    close(fd2[0]);
    write(fd2[1], str, SIZE);
    close(fd2[1]);

    // Take concatenated string from parent
    close(fd1[1]);
    read(fd1[0], concat_str, SIZE);
    close(fd1[0]);
    
    printf("The concatenated string is: ");
    fflush(stdout);
    fputs(concat_str, stdout);
    printf("\n");
    exit(0);
  }
  else {
    char str2[SIZE];  
    memset(str2, 0, SIZE);
    get_input(str, "the main string");  
    FreeChild(fd3);  

    // Get the string2 from the child 
    close(fd2[1]);
    read(fd2[0], str2, SIZE);
    close(fd2[0]);

    strcat(str, str2);
    
    // Send the concatenated string to the child
    close(fd1[0]);
    write(fd1[1], str, SIZE);
    close(fd1[1]);
  }
  return 0;
}

// Frees the Child which is waiting(Sync)
void FreeChild(int * fd) {
  int let_child_wait = 0;  
  close(fd[0]);
  write(fd[1], &let_child_wait, 4);
  close(fd[1]);  
}

// Let me wait for the parent to be done(Sync)
int WaitParent(int * fd) {
  int wait_for_parent = 1;
  close(fd[1]);
  read(fd[0], &wait_for_parent, 4);
  close(fd[0]);
  return wait_for_parent;
}

// Take input string in 'str'
void get_input(char * str, char * msg) {
  printf("Enter %s: \n", msg);
  fgets(str, SIZE, stdin);
  RipOffNextLine(str);
}

// A simple utility to rip off the trailing '\n' from 'str'
void RipOffNextLine(char * str) {
  int i = 0;
  while(str[i] != '\n') 
    i++;
  str[i] = '\0';
}
