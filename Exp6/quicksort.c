#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>

// A package to pass around data among threads
struct block {
  int * arr;  // the array
  int size;   // the size of the array
  int start;  // the start index     
  int end;    // the end index
};

void swap_ints(int* x, int* y);
void QuickSort(int * arr, int start, int end);
int PlacePivotCorrectly(int * arr, int start, int end);
void * runner(void * params);

int main  () {
  int size;
  printf("Enter Array size: "); 
  scanf("%d", &size);
  int arr[size];
  for (int i = 0; i < size; i++) 
    scanf("%d", &arr[i]);
  
  QuickSort(arr, 0, size - 1);
  // struct block args;
  // args.arr = arr;
  // args.size = size;
  // args.start = 0;
  // args.end = size - 1;
  // pthread_t tid;
  // pthread_create(&tid, NULL, runner, &args);
  // pthread_join(tid, NULL);
  printf("\nSorted array is \n"); 
  for (int i = 0; i < size; i++)
    printf("%d ", arr[i]);
  printf("\n");
  return 0;
} 


void QuickSort(int * arr, int start, int end) {
  if (start < end) {
    int corrected_index = PlacePivotCorrectly(arr, start, end);
    QuickSort(arr, start, corrected_index - 1);
    QuickSort(arr, corrected_index + 1, end);
  }
  return;
}

// Places the pivot element appropriately by partitioning the array and return the new pivot index
int PlacePivotCorrectly(int * arr, int start, int end) {
  int i = start - 1;
  int j = start;
  int pivot_id = end;
  int pivot = arr[pivot_id];
  while(j <= pivot_id - 1) {
    if(arr[j] < pivot) {
      i++;
      swap_ints(&arr[i], &arr[j]);
    }
    j++;
  } 
  swap_ints(&arr[i + 1], &arr[pivot_id]);
  return i + 1;  // this is new pivot_id
}

// swaps two integers
void swap_ints(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

