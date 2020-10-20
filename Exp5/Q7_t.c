#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<pthread.h>
 
// A structure to pass data among threads
struct block {
  int * array;     // the array input
  int * hot;       // the hot array
  int key;         // the key to be searched
  int start;       // searching start index
  int end;          // searching end index
  char msg[64];      // "first" or "second"
};

void* runner_main(void * params);
void* runner(void * params);
void asc_sort(int * arr, int start, int end, char * msg);
void print(int * arr, int start, int end);
void my_swap(int* x, int* y);
void binary_search(int arr[],int key, int l, int h, int hot[]);
void show(int hot[], int l, int h);

int n; //  number of integers

int main() {
  int key;
  pthread_t tid1, tid2;
  pthread_attr_t attr;
  printf("Enter the number of integers: ");
  scanf("%d", &n);
  int arr[n];
  printf("Enter the array:");
  for(int i = 0; i < n; i++)
    scanf("%d", &arr[i]);
  printf("Enter the key to be searched: ");
  scanf("%d", &key);
  int hot[n];
  for (int i = 0; i < n; i++) hot[i] = 0;

  struct block main, left_thread, right_thread;
  main.array = arr;
  main.hot = hot;
  main.key = key;
  
  left_thread = main;
  left_thread.start = 0;
  left_thread.end = n / 2 - 1;
  strcpy(left_thread.msg, "first");

  right_thread = main;
  right_thread.start = n / 2;
  right_thread.end = n - 1;
  strcpy(right_thread.msg, "second");
  

  pthread_attr_init(&attr);
  pthread_create(&tid1, &attr, runner_main, &left_thread);
  pthread_create(&tid2, &attr, runner_main, &right_thread);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  return 0;
}

// Multithreading binary search version for all occurrences of key
void binary_search(int arr[],int key, int l, int h, int hot[]) {
  if (l > h)     // Base case
    return;
  int mid = (l + h) / 2;
  if (arr[mid] == key) {
    hot[mid] = 1;
    pthread_t tid1, tid2;
    pthread_attr_t attr;
    struct block left, right, main;
    main.array = arr;
    main.key = key;
    main.hot = hot;
    
    // setting up arguments for the left thread
    left = main;
    left.start = l;
    left.end = mid - 1;

    // setting up arguments for the right thread
    right = main;
    right.start = mid + 1;
    right.end = h;

    pthread_attr_init(&attr);

    // Calculate fib(n-1) and fib(n-2) by initiating thread1 and thread2
    pthread_create(&tid1, &attr, runner, &left); // search 'key' in left half
    pthread_create(&tid2, &attr, runner, &right); // search 'key' in right half
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_exit(NULL);
    // return;
  } 
  else if (arr[mid] < key) 
    return binary_search(arr, key, mid + 1, h, hot);
  else  
    return binary_search(arr, key, l, mid - 1, hot);
}

/* Thread routine which sorts and binary searches in 
 one(left/right) part of the input array */
void* runner_main(void * params) {
  struct block * args = params;
  asc_sort(args->array, args->start, args->end, args->msg);
  binary_search(args->array, args->key, args->start, args->end, args->hot);
  printf("The key is present at these locations in the %s half of sorted array: \n", args->msg);
  show(args->hot, args->start, args->end);
  pthread_exit(NULL);
}

// Thread routine which binary searches in one(left/right) part of the array
void* runner(void * params) {
  struct block * args = params;
  binary_search(args->array, args->key, args->start, args->end, args->hot);
  pthread_exit(NULL);
}

// print the hot[] location with 1 entries (Printing the Search data)
void show(int hot[], int l, int h) {
  int flag = 0;
  for (int i = l; i <= h; i++) {
    if (hot[i] == 1) {
      flag = 1;
      printf("%d ", i);
    }
  }
  if (flag == 0)
    printf("\nNo such location in this part of the array.");
  printf("\n");
}

// Ascending order sorting and printing
void asc_sort(int * arr, int start, int end, char * msg) {
  for (int i = start; i < end; i++) {
    for (int j = start; j < end + start - i; j++) {
      // printf("hello 22 %d\n", arr[j]);
      if (arr[j] > arr[j + 1]) {
        // printf("hello 2 %d\n", i);
        my_swap(&arr[j], &arr[j + 1]);
      }
    }
  }
  printf("\nSorted %s half Array in Ascending Order: ", msg);
  print(arr, start, end);
}

// swap two integer variables
void my_swap(int* x, int* y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

// print the array in range [start, end]
void print(int * arr, int start, int end) {
  for(int i = start; i <= end; i++) 
    printf("%d ", arr[i]);
  printf("\n");
}