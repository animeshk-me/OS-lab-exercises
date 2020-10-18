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
  int fd[2];
  for(int i = 0; i < n; i++)
    scanf("%d", &arr[i]);
  pipe(fd);
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    desc_sort(arr, n);
    close(fd[0]);
    write(fd[1], arr, sizeof(arr));
    close(fd[1]);
    exit(0);
  }
  else {           // Parent block
    int desc_sorted_arr[n];
    printf("Parent Block*****************\n");
    asc_sort(arr, n);
    close(fd[1]);
    read(fd[0], desc_sorted_arr, sizeof(desc_sorted_arr));
    close(fd[0]);
    printf("Sorted Array in Ascending Order: ");
    print(arr, n);
    printf("Sorted Array in Descending Order: ");
    print(desc_sorted_arr, n);
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
}

void desc_sort(int * arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] < arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
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