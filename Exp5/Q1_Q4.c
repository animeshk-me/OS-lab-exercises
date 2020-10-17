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

void get_input(char * str);
void RipOffNextLine(char * str);
void Reverse(char * str);

int main() {
  char str[SIZE];
  int fd1[2];  // Parent writing and Child Reading
  int fd2[2];  // Child writing and Parent Reading
  pipe(fd1);
  pipe(fd2);
  memset(str, 0, SIZE);
  pid_t pid = fork();
  if (pid == 0) {  // Child Process
    // Take string from parent
    close(fd1[1]);
    read(fd1[0], str, SIZE);
    close(fd1[0]);
    
    Reverse(str);

    // Give back the reversed string to parent
    close(fd2[0]);
    write(fd2[1], str, SIZE);
    close(fd2[1]);
    exit(0);
  }
  else {
    char rev_str[SIZE];
    memset(rev_str, 0, SIZE);
    get_input(str);    
   
    // Send the read string to the child
    close(fd1[0]);
    write(fd1[1], str, SIZE);
    close(fd1[1]);

    // Get the reversed string from the child 
    close(fd2[1]);
    read(fd2[0], rev_str, SIZE);
    close(fd2[0]);

    printf("The string '%s' has the reverse string '%s'. ", str, rev_str);
    if (!strcmp(str, rev_str))
      printf("Therefore it is a palindrome.\n");
    else
      printf("Therefore it is not a palindrome.\n");
  }

  return 0;
}

// Take input string in 'str'
void get_input(char * str) {
  printf("Enter a String(all in lower case): \n");
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

// Revers the input string 'str'
void Reverse(char * str) {
  char rev_str[SIZE];
  memset(rev_str, 0, SIZE);
  int i = 0;
  int j = 0;
  while(str[i] != '\0')
    i++;
  i--;
  while(i >= 0) {
    rev_str[j] = str[i];
    i--;
    j++;
  }
  strcpy(str, rev_str);
  // fputs(rev_str, stdout);
}