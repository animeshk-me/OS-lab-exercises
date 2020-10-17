#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<fcntl.h>

char History[10][100];
void Record(char * cmd);
void Print_tot();
void RipOffNextLine(char * buff);

int main() {
  char cmd[25];
  while(1) {
    fgets(cmd, sizeof(cmd), stdin);
    RipOffNextLine(cmd);
    Record(cmd);
  }
  return 0;
}

void Record(char * cmd) {
  for (int i = 8; i >= 0; i--)
    strcpy(History[i + 1], History[i]);
  strcpy(History[0], cmd);
  Print_tot();
}

void Print_tot() {
  for (int i = 0; i < 10; i++)
    printf("#%d ho %s\n", i, History[i]);
}

// A simple utility to rip off the trailing '\n' from 'buff'
void RipOffNextLine(char * buff) {
  int i = 0;
  while(buff[i] != '\n') 
    i++;
  buff[i] = '\0';
}