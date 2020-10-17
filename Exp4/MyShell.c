#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

char History[9][100];

void PrintHistory(char * args);
void Record(char * cmd);
void Parser(char * buff, char**args);
void RipOffNextLine(char * buff);
void get_pwd_decorator(char * buff);

int main() {
  while(1) {
    char buff[256];
    char * args[50];     
    char curr_path[256]; 
    int status = 0;
    pid_t wpid;
    get_pwd_decorator(curr_path);
    printf("\n[+] [Animesh@CED18I065]─[%s]\n└──╼ # ", curr_path);
    fgets(buff, sizeof(buff), stdin);
    if(strcmp(buff, "\n") == 0) // In case of empty string
      continue;
    Parser(buff, args);
    Record(args[0]);

    if (args[0][0] == '!')  // History Command 
      PrintHistory(args[0]);

    else if (strcmp(args[0], "exit()") == 0) { // For exit()
      printf("Thanks for using.\nCommand Shell Created by ANIMESH KUMAR CED18I065 2020\n");
      break;
    }

    else if (strcmp(args[0], "help") == 0) // Special care for 'help'
      printf("1. Use exit() to exit the Command Prompt\n2. The Pipes(|) do not work\n3. Enter !n, for history, where 0 < n < 10\n");

    else if (strcmp(args[0], "cd") == 0) // Special care for 'cd' 
      chdir(args[1]);

    else if (fork() == 0) {   // Child block which executes commands
      if (execvp(args[0], args) < 0)
        printf("[✗]Exec failure: Invalid Input\nEnter 'help' for help\n");
      exit(0);
    }
    
    else         // Parent waits for child to complete
      while ((wpid = wait(&status)) > 0);
  } // Infinite loop ends
  return 0;
}

// Prints the commands executed in history
void PrintHistory(char * args) {
  if ((args[1] > 57) || (args[1] < 49) || (args[2] != '\0')) {
    printf("[✗]History failure: Invalid Input\nEnter 'help' for help\n");
    return;
  }
  for (int i = 0; i <= args[1] - 49; i++) {
    printf("%s\n", History[i]);
  }
}

// Records the command in the History book
void Record(char * cmd) {
  for (int i = 8; i >= 0; i--) // Making space for the new record
    strcpy(History[i + 1], History[i]);
  strcpy(History[0], cmd);
}


// For decorating the prompt with the 'pwd' output
void get_pwd_decorator(char * buff) {
  int fd[2];
  pipe(fd);
  if (fork() == 0) { // Child Process
    close(fd[0]);
    dup2(fd[1], 1); // to send the output of 'pwd' to the parent via pipe
    execlp("pwd", "pwd", NULL);
  }
  else {             // Parent Process
    close(fd[1]);
    read(fd[0], buff, 256);
    close(fd[0]);
    RipOffNextLine(buff); // rip off the trailing '\n'
  }
}

// Parses the 'buff' into an array of strings 'args'
void Parser(char * buff, char**args) {
  int i = 0;
  RipOffNextLine(buff);
  char * token = strtok(buff, " ");
  args[i] = token;
  i++;
  while(1) {
    token = strtok(NULL, " ");
    if (token == NULL)
      break;
    args[i] = token;
    i++;
  }
  if (strcmp(args[0], "ls") == 0) {
    args[i] = "--color=auto";
    i++;
  }
  args[i] = (char *)NULL;
}

// A simple utility to rip off the trailing '\n' from 'buff'
void RipOffNextLine(char * buff) {
  int i = 0;
  while(buff[i] != '\n') 
    i++;
  buff[i] = '\0';
}