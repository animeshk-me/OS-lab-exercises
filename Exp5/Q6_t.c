#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>

#define ASC 1     // for ascending order
#define DESC 0    // for descending order

// A structure to hold the data to be passed to the threads
struct block {
  int * arr;    // the original array
  int arr_size; // the arr_size
  int * mod_arr; // the sorted array
  int order;    // ASC or DESC
};

void arrncpy(int * dest, int * src, int size);
void* runner(void* params);
void desc_sort(int * arr, int size);
void asc_sort(int * arr, int size);
void print(int * arr, int size);
void my_swap(int* x, int* y);

int main() {
  int n;
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  struct block first, second;
  printf("Enter the number of integers: ");
  scanf("%d", &n);
  int arr[n], asc_sorted[n], desc_sorted[n];
  for(int i = 0; i < n; i++)
    scanf("%d", &arr[i]);

  // Setting up the parameters to be passed to the threads
  first.arr = arr;
  first.arr_size = n;
  first.order = ASC;
  first.mod_arr = asc_sorted;

  second.arr = arr;
  second.arr_size = n;
  second.order = DESC;
  second.mod_arr = desc_sorted;

  pthread_attr_init(&attr);

  // Creating the two threads
  pthread_create(&tid1, &attr, runner, &first);  // for ascending sort
  pthread_create(&tid2, &attr, runner, &second); // for descending sort
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Sorted Array in Ascending Order: ");
  print(first.mod_arr, n);
  printf("Sorted Array in Descending Order: ");
  print(second.mod_arr, n);
  return 0;
}

// The thread routine which triggers asc_sort/desc_sort based on 'params'
void* runner(void* params) {
  struct block * args = params;
  arrncpy(args->mod_arr, args->arr, args->arr_size);
  if (args->order == ASC) 
    asc_sort(args->mod_arr, args->arr_size);
  else 
    desc_sort(args->mod_arr, args->arr_size);
  pthread_exit(NULL);
}

// Copy SRC to DEST array of SIZE
void arrncpy(int * dest, int * src, int size) {
  for (int i = 0; i < size; i++) 
    dest[i] = src[i];
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

// Sorting in descending order
void desc_sort(int * arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] < arr[j + 1])
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

// print the array arr
void print(int * arr, int size) {
  for(int i = 0; i < size; i++) 
    printf("%d ", arr[i]);
  printf("\n");
}