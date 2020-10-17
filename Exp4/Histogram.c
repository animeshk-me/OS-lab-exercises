#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>

void CreateHistogram(char * filename);

int main  () {
  char * filename = "text_file.txt";
  CreateHistogram(filename);  
  return 0;
} 

void CreateHistogram(char * filename) { 
  __pid_t pid1, pid2, pid3, pid4, pid5, pid6, pid7, pid8, pid9, pid10, pid11, pid12, pid13, pid14, pid15, pid16, pid17, pid18, pid19, pid20, pid21, pid22, pid23, pid24, pid25, wpid;
  int status = 0;

  // Opening the text file to read the text input.
  FILE * fptr = fopen(filename, "r");

 /* We need to fork 25 new child processes, each of them counting the frequency 
  of a single alphabet(smaller case)*/
  ((pid1 = fork()) && (pid2 = fork()) && (pid3 = fork()) && (pid4 = fork()) && (pid5 = fork()) && (pid6 = fork()) && (pid7 = fork()) && (pid8 = fork()) && (pid9 = fork()) && (pid10 = fork()) && (pid11 = fork()) && (pid12 = fork()) && (pid13 = fork()) && (pid14 = fork()) && (pid15 = fork()) && (pid16 = fork()) && (pid17 = fork()) && (pid18 = fork()) && (pid19 = fork()) && (pid20 = fork()) && (pid21 = fork()) && (pid22 = fork()) && (pid23 = fork()) && (pid24 = fork()) && (pid25 = fork()));

// Computes the frequency of a's
  if (pid1 == 0) {
    int count = 0;
    printf("a: ");
    rewind(fptr);
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'a') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
// Computes the frequency of b's and so on ...
  else if (pid2 == 0) {
    int count = 0;
    printf("b: ");
    rewind(fptr);
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'b') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid3 == 0) {
    int count = 0;
    rewind(fptr);
    printf("c: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'c') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid4 == 0) {
    int count = 0;
    rewind(fptr);
    printf("d: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'd') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid5 == 0) {
    int count = 0;
    rewind(fptr);
    printf("e: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'e') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid6 == 0) {
    int count = 0;
    rewind(fptr);
    printf("f: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'f') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid7 == 0) {
    int count = 0;
    rewind(fptr);
    printf("g: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'g') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid8 == 0) {
    int count = 0;
    rewind(fptr);
    printf("h: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'h') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid9 == 0) {
    int count = 0;
    rewind(fptr);
    printf("i: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'i') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid10 == 0) {
    int count = 0;
    rewind(fptr);
    printf("j: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'j') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid11 == 0) {
    int count = 0;
    rewind(fptr);
    printf("k: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'k') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid12 == 0) {
    int count = 0;
    rewind(fptr);
    printf("l: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'l') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid13 == 0) {
    int count = 0;
    rewind(fptr);
    printf("m: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'm') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid14 == 0) {
    int count = 0;
    rewind(fptr);
    printf("n: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'n') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid15 == 0) {
    int count = 0;
    rewind(fptr);
    printf("o: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'o') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid16 == 0) {
    int count = 0;
    rewind(fptr);
    printf("p: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'p') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid17 == 0) {
    int count = 0;
    rewind(fptr);
    printf("q: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'q') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid18 == 0) {
    int count = 0;
    rewind(fptr);
    printf("r: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'r') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid19 == 0) {
    int count = 0;
    rewind(fptr);
    printf("s: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 's') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid20 == 0) {
    int count = 0;
    rewind(fptr);
    printf("t: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 't') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid21 == 0) {
    int count = 0;
    rewind(fptr);
    printf("u: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'u') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid22 == 0) {
    int count = 0;
    rewind(fptr);
    printf("v: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'v') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid23 == 0) {
    int count = 0;
    rewind(fptr);
    printf("w: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'w') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid24 == 0) {
    int count = 0;
    rewind(fptr);
    printf("x: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'x') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else if (pid25 == 0) {
    int count = 0;
    rewind(fptr);
    printf("y: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'y') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
  }
  else {
    while ((wpid = wait(&status)) > 0);
    int count = 0;
    rewind(fptr);
    printf("z: ");
    char ch = fgetc(fptr); 
    while (ch != EOF) { 
      if (ch == 'z') {
        printf("[_]");
        count++;
      }
      ch = fgetc(fptr); 
    }
    printf("     (%d)\n", count);
    printf("\n************The Histogram of Occurences of all the Characters**********\n");
  } 
  
} 
