#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
 
int main() {
  int n;
  printf("Enter the total number of terms: ");
  scanf("%d", &n);
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    printf("The even series: ");
    if ((n % 2) != 0)
      n -= 1;
    while(n > 0) {
      printf("%d ", n);
      n -= 2;
    }
    printf("\n");
  }
  else {           // Parent block
    printf("The odd series: ");
    if ((n % 2) == 0)
      n -= 1;
    while(n > 0) {
      printf("%d ", n);
      n -= 2;
    }
    printf("\n");
  }
  return 0;
}