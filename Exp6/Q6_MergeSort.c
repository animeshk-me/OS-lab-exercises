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

void merge(int arr[], int l, int m, int r);
void * runner(void * params);

int main  () {
  int size;
  printf("Enter Array size: "); 
  scanf("%d", &size);
  int arr[size];
  for (int i = 0; i < size; i++) 
    scanf("%d", &arr[i]);
  
  struct block args;
  args.arr = arr;
  args.size = size;
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

// This routine recursively implements multithreading version of Mergesort
void *runner(void * params) {
  struct block * args = params;
  if (args->start < args->end) {
    pthread_t tid[2];
    int mid = args->start + (args->end - args->start) / 2;
    struct block args_left, args_right;
    args_left.arr = args->arr;
    args_left.size = args->size;
    args_left.start = args->start;
    args_left.end = mid;
  
    args_right.arr = args->arr;
    args_right.size = args->size;
    args_right.start = mid + 1;
    args_right.end = args->end;

    // Recursive thread creation to sort left and right halves
    pthread_create(&tid[0], NULL, runner, &args_left);
    pthread_create(&tid[1], NULL, runner, &args_right);
    // wait for both the threads to complete
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL); 

    // merging the sorted halves
    merge(args->arr, args->start, mid, args->end);
  }
  pthread_exit(NULL);
}



// The usual Merge routine to merge the sorted halves of the array arr[]
void merge(int arr[], int l, int m, int r) 
{ 
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 = r - m; 
	int L[n1], R[n2]; 
	for (i = 0; i < n1; i++) 
		L[i] = arr[l + i]; 
	for (j = 0; j < n2; j++) 
		R[j] = arr[m + 1 + j]; 
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 

	while (i < n1 && j < n2) { 
		if (L[i] <= R[j]) { 
			arr[k] = L[i]; 
			i++; 
		} 
		else { 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 
  // for remaining elements
	while (i < n1) { 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 
  // for remaining elements
	while (j < n2) { 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
} 
