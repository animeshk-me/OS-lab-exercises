#include "func_header.h"

// #define SIZE 100

int main() {
  char str1[SIZE];
  char str2[SIZE];
  int indices[2];
  memset(str1, 0, SIZE);
  memset(str2, 0, SIZE);
  get_input(str1, "first");
  get_input(str2, "second");
  len1 = strlen(str1);
  len2 = strlen(str2);
  printf("The length of the longest common subsequence: %d\n", LCS(str1, str2));
  return 0;
}