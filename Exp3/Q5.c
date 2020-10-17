#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
 

void desc_sort(int * arr, int size);
void asc_sort(int * arr, int size);
void print(int * arr, int size);
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
    desc_sort(arr, n);
  }
  else {           // Parent block
    printf("Parent Block*****************\n");
    asc_sort(arr, n);
  }
  return 0;
}

void asc_sort(int * arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
  printf("Sorted Array in Ascending Order: ");
  print(arr, size);
}

void desc_sort(int * arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] < arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
  printf("Sorted Array in Ascending Order: ");
  print(arr, size);
}

void my_swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

void print(int * arr, int size) {
  for(int i = 0; i < size; i++) 
    printf("%d ", arr[i]);
  printf("\n");
}