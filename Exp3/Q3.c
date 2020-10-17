#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
 
int arm(int n);
int Pow(int n, int x);

int main() {
  int a, b;
  printf("Enter a and b for the range (a, b) both inclusive: ");
  scanf("%d %d", &a, &b);
  for (int i = a; i <= b; i++) {
    if (arm(i))
      printf("%d ", i);   
  }
  printf("\n"); 
  return 0;
}

int arm(int n) {
  int sum = 0;
  int i = n;
  int len = 0;
  while(i != 0) {
    i /= 10;
    len++;
  }
  i = n;
  while(i != 0) {
    sum += Pow((i % 10), len);
    i /= 10;
  }
  if (sum == n)
    return 1;
  return 0;
}

int Pow(int n, int x) {
  if (x == 0)   return 1;
  int prod = n;
  while(x > 1) {
    prod *= n;
    x--;
  }
  return prod;
}