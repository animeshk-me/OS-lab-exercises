#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>

#define INDEX_NOT_FOUND -1

// A data package to pass arguments around threads
struct block {
  int * arr;  // the array
  int start;   // the size of array 'arr'
  int end;
  int sum_val; // sum of the array from 'start' to 'end'
};

// A data package which will be used to calculate MODE
struct number {
  int self;     // the number itself
  int frequency; // the frequency of the number
};

void* runner1(void* params);
void* runner2(void* params);
void* runner3(void* params);
void* runner_sum(void * params);
void PrintMode(struct number * Numbers, int size);
int get_index(int n, struct number * Numbers, int size);
void Initialize(struct number * Numbers, int size);
void asc_sort(int * arr, int size);
void my_swap(int* x, int* y);

int main() {
  int size;
  printf("Enter the number of elements: ");
  scanf("%d", &size);
  int arr[size];
  for (int i = 0; i < size; i++) {
    scanf("%d", &arr[i]);
  }
  struct block args;
  args.arr = arr;
  args.start = 0;
  args.end = size - 1;

  void* (*runners[3])(void * params) = {runner1, runner2, runner3};
  pthread_t tid[3];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  for (int i = 0; i < 3; i++) {
    pthread_create(&tid[i], &attr, (*runners[i]), &args);
  }
  for (int i = 0; i < 3; i++) {
    pthread_join(tid[i], NULL);
  }
  return 0;
}

/******************** Mean Calculation *************************/
// This thread calculates the mean
void* runner1(void* params) {
  struct block * args = params;
  pthread_t tid;
  pthread_create(&tid, NULL, runner_sum, args); // calculate the sum first
  pthread_join(tid, NULL);
  printf("Mean: %f\n", (float)args->sum_val / (float)(args->end + 1));
  pthread_exit(NULL);
}

// this thread routine finds sum of the array using divide and conquer
void* runner_sum(void * params) {
  struct block * array = params;
  if (array->start == array->end) {
    array->sum_val = array->arr[array->start];
    pthread_exit(NULL);
  }
  struct block array_left, array_right;
  int mid = (array->start + array->end) / 2;
  array_left.arr = array->arr;
  array_right.arr = array->arr;
  array_left.start = array->start;
  array_left.end = mid;
  array_right.start = mid + 1;
  array_right.end = array->end;

  // Find the sum of left and right parts of the array
  pthread_t tid1, tid2;
  pthread_create(&tid1, NULL, runner_sum, &array_left);
  pthread_create(&tid2, NULL, runner_sum, &array_right);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  array->sum_val = array_left.sum_val + array_right.sum_val;
  pthread_exit(NULL);
}

/***************************************************************/


/******************** Mode Calculation *************************/

// this calculates the mode of the numbers
void* runner2(void* params) {
  struct block * args = params;
  // Frequency calculation
  struct number Numbers[args->end + 1];
  Initialize(Numbers, args->end + 1);
  int j = 0;
  for (int i = args->start; i <= args->end; i++) {
    int id = get_index(args->arr[i], Numbers, args->end + 1);
    if (id == INDEX_NOT_FOUND) {
      Numbers[j].self = args->arr[i];
      Numbers[j].frequency = Numbers[j].frequency + 1;
      j++;
    }
    else 
      Numbers[id].frequency = Numbers[id].frequency + 1;
  }
  PrintMode(Numbers, args->end + 1);
  pthread_exit(NULL);
}

// Initializing the Numbers array
void Initialize(struct number * Numbers, int size) {
  for (int i = 0; i < size; i++) {
    Numbers[i].self = -1;
    Numbers[i].frequency = 0;
  }
}

// Returns mode of the input Numbers and their frequencies
void PrintMode(struct number * Numbers, int size) {
  int max_frequency = 0;
  for (int i = 0; i < size; i++) {
    max_frequency = Numbers[i].frequency > max_frequency ? Numbers[i].frequency : max_frequency;
  }
  printf("Mode (there may be 0 or more modes possible): ");
  if (max_frequency == 1) 
    printf("There is no mode of the entered data.\n");
  else { // One or more than one mode exist
    for (int i = 0; i < size; i++) {
      if (Numbers[i].frequency == max_frequency)
        printf("%d ", Numbers[i].self);
    }
    printf("\n");
  }  
}

// Returns the index of the N if it is found in Numbers[]
int get_index(int n, struct number * Numbers, int size) {
  for (int i = 0; i < size; i++) {
    if (Numbers[i].self == n)
      return i;
  }
  return INDEX_NOT_FOUND; 
}

/***************************************************************/
 

/****************** Median Calculation *************************/

// this thread routine calculates median of the integers
void* runner3(void* params) {
  struct block * args = params;
  int size = args->end + 1;
  asc_sort(args->arr, size);
  if (size % 2 == 0) {
    float median = (float)(args->arr[size / 2] + args->arr[size / 2 - 1]) / (float)2;
    printf("\nMedian: %f\n", median);
  }
  else  // for odd length array
    printf("\nMedian: %d\n", args->arr[size / 2]);
  pthread_exit(NULL);
}

// Sorting in ascending order
void asc_sort(int * arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1])
        my_swap(&arr[j], &arr[j + 1]);
    }
  }
}

// swap two integers
void my_swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

/***************************************************************/