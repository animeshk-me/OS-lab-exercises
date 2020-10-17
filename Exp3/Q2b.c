#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
 
int main() {
  int n;
  printf("Enter the total number of terms: ");
  scanf("%d", &n);
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    printf("The sum of even terms: ");
    int sum = 0;
    if ((n % 2) != 0)
      n -= 1;
    while(n > 0) {
      // printf("%d ", n);
      sum += n;
      n -= 2;
    }
    printf("The sum of even terms: %d\n", sum);
  }
  else {           // Parent block
    int sum = 0;
    if ((n % 2) == 0)
      n -= 1;
    while(n > 0) {
      // printf("%d ", n);
      sum += n;
      n -= 2;
    }
    printf("The sum of odd terms: %d\n", sum);
  }
  return 0;
}