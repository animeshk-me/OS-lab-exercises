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
  int start;  // the start index     
  int end;    // the end index
};

void swap_ints(int* x, int* y);
int PlacePivotCorrectly(int * arr, int start, int end);
void * runner(void * params);

int main() {
  int size;
  printf("Enter Array size: "); 
  scanf("%d", &size);
  int arr[size];
  for (int i = 0; i < size; i++) 
    scanf("%d", &arr[i]);
  
  struct block args;
  args.arr = arr;
  args.start = 0;
  args.end = size - 1;
  pthread_t tid;
  pthread_create(&tid, NULL, runner, &args);
  pthread_join(tid, NULL);
  printf("\nSorted array is \n"); 
  for (int i = 0; i < size; i++)
    printf("%d ", args.arr[i]);
  printf("\n");
  return 0;
} 

// This routine recursively implements multithreading version of Quicksort
void *runner(void * params) {
  struct block * args = params;
  if (args->start < args->end) {
    pthread_t tid[2];
    int corrected_index = PlacePivotCorrectly(args->arr, args->start, args->end);

    struct block args_left, args_right;
    args_left.arr = args->arr;
    args_left.start = args->start;
    args_left.end = corrected_index - 1;
  
    args_right.arr = args->arr;
    args_right.start = corrected_index + 1;
    args_right.end = args->end;

    // Recursive thread creation to sort left and right halves
    pthread_create(&tid[0], NULL, runner, &args_left);
    pthread_create(&tid[1], NULL, runner, &args_right);
    // wait for both the threads to complete
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL); 
  }
  pthread_exit(NULL);
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

