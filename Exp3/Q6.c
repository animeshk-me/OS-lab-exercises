#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
 

void desc_sort(int * arr, int start, int end);
void asc_sort(int * arr, int start, int end);
void print(int * arr, int start, int end);
void my_swap(int* x, int* y);

int main() {
  int n;
  printf("Enter the number of integers: ");
  scanf("%d", &n);
  int arr[n];
  for(int i = 0; i < n; i++)
    scanf("%d", &arr[i]);
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    printf("Child Block******************\n");
    desc_sort(arr, n / 2, n - 1);
  }
  else {           // Parent block
    printf("Parent Block*****************\n");
    asc_sort(arr, 0, n / 2 - 1);
  }
  return 0;
}

void asc_sort(int * arr, int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = start; j < end + start - i; j++) {
      if (arr[j] > arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
  printf("Sorted half Array in Ascending Order: ");
  print(arr, start, end);
}

void desc_sort(int * arr, int start, int end) {
  for (int i = start; i < end; i++) {
    for (int j = start; j < end + start - i; j++) {
      if (arr[j] < arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
  printf("Sorted half Array in Descending Order: ");
  print(arr, start, end);
}

void my_swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

void print(int * arr, int start, int end) {
  for(int i = start; i <= end; i++) 
    printf("%d ", arr[i]);
  printf("\n");
}