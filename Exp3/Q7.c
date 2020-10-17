#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
 
void asc_sort(int * arr, int start, int end);
void print(int * arr, int start, int end);
void my_swap(int* x, int* y);
void binary_search(int arr[],int key, int l, int h, int hot[]);
void show(int hot[], int l, int h);

int main() {
  int n, key;
  printf("Enter the number of integers: ");
  scanf("%d", &n);
  int arr[n];
  for(int i = 0; i < n; i++)
    scanf("%d", &arr[i]);
  printf("Enter the key to be searched: ");
  scanf("%d", &key);
  int hot[n];
  for (int i = 0; i < n; i++) hot[i] = 0;
  pid_t pid = fork();
  if (pid == 0) {  // Child block 
    printf("******************Child Block******************\n\n");
    asc_sort(arr, n / 2, n - 1);
    printf("The key is present at these locations in the second half of sorted array: ");
    binary_search(arr, key, n / 2, n - 1, hot);
    show(hot, n / 2, n - 1);
    printf("\n\n**********************************************\n");
  }
  else {           // Parent block
    printf("*****************Parent Block*****************\n\n");
    asc_sort(arr, 0, n / 2 - 1);
    printf("The key is present at these locations in the first half of sorted array: ");
    binary_search(arr, key, 0, n / 2 - 1, hot);
    show(hot, 0, n / 2 - 1);
    printf("\n\n**********************************************\n");
  }
  return 0;
}

// binary search for all occurrences of key
void binary_search(int arr[],int key, int l, int h, int hot[]) {
  if (l > h)
    return;
  int mid = (l + h) / 2;
  if (arr[mid] == key) {
    hot[mid] = 1;
    binary_search(arr, key, l, mid - 1, hot);
    binary_search(arr, key, mid + 1, h, hot);
    return;
  } 
  else if (arr[mid] < key)
    return binary_search(arr, key, mid + 1, h, hot);
  else
    return binary_search(arr, key, l, mid - 1, hot);
}

// print the hot[] location with 1 entries
void show(int hot[], int l, int h) {
  int flag = 0;
  for (int i = l; i <= h; i++) {
    if (hot[i] == 1) {
      flag = 1;
      printf("%d ", i);
    }
  }
  if (flag == 0)
    printf("No such location in this part of the array.");
  printf("\n");
}

// Ascending order sorting
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

// swap two integer variables
void my_swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

// print the input array
void print(int * arr, int start, int end) {
  for(int i = start; i <= end; i++) 
    printf("%d ", arr[i]);
  printf("\n");
}