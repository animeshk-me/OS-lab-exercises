#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

#define SIZE 100

void get_substring(char * str, char * substring, int * indices);
void get_input(char * str, int * indices);
void RipOffNextLine(char * str);

int main() {
  char str[SIZE];
  int indices[2];
  memset(str, 0, SIZE);
  int fd1[2];  
  int fd2[2]; 
  int fd3[2];  // For Synchronizing the input from stdin by the two processes.
  pipe(fd1);
  pipe(fd2);
  pipe(fd3);
  pid_t pid = fork();
  if (pid == 0) {  // Child Process
    // Take string from parent
    close(fd1[1]);
    read(fd1[0], str, SIZE);
    close(fd1[0]);
    
    // Take indices from parent
    close(fd2[1]);
    read(fd2[0], indices, 8);
    close(fd2[0]);

    char substring[indices[1] - indices[0] + 2];
    get_substring(str, substring, indices);

    // Send the generated substring
    close(fd3[0]);
    write(fd3[1], substring, indices[1] - indices[0] + 2);
    close(fd3[1]);
    exit(0);
  }
  else {             // Parent Process
    get_input(str, indices);  
    char substring[indices[1] - indices[0] + 2];
    // Send the string to the child
    close(fd1[0]);
    write(fd1[1], str, SIZE);
    close(fd1[1]);

    // Send the indices to the child
    close(fd2[0]);
    write(fd2[1], indices, 8);
    close(fd2[1]);
    
    // Get the substring from the child 
    close(fd3[1]);
    read(fd3[0], substring, indices[1] - indices[0] + 2);
    close(fd3[0]);

    printf("The requested substring is: ");
    fputs(substring, stdout);
    printf("\n");
  }
  return 0;
}

// Take input string in 'str' and get indices of the substring
void get_input(char * str, int * indices) {
  printf("Enter the string: \n");
  fgets(str, SIZE, stdin);
  RipOffNextLine(str);
  printf("Enter the first and second index respectively: \n");
  scanf("%d %d", &indices[0], &indices[1]);
}

// A utility which returns required substring in 'substring'
void get_substring(char * str, char * substring, int * indices) {
  strncpy(substring, str + indices[0], indices[1] - indices[0] + 1);
  substring[indices[1] - indices[0] + 1] = '\0';
}


// A simple utility to rip off the trailing '\n' from 'str'
void RipOffNextLine(char * str) {
  int i = 0;
  while(str[i] != '\n') 
    i++;
  str[i] = '\0';
}
