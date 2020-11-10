#include<stdio.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>
#include<signal.h>

#include<stdlib.h> 
#define SIZE 7

int real_size = 0; // the real size of the dictionary at current time
int found = 0;

// A single word info
struct word {
  char self[20];    // the word
  char mean1[20];   // the first meaning
  char mean2[20];   // the second meaning
};

// A package to pass data among threads
struct block {
  struct word * Words; // Dictionary array(of word's)
  int l;               // left end of Words[]
  int h;               // right end of Words[]
  char keyword[20];      // keyword to be searched
  pthread_t *other_tid; // tid of the sibling thread
};

void print_dict(struct word * head);
int add_word(struct word * Words);
void binary_search(struct word * Words, char * keyword, int l, int h, pthread_t *other_tid);
void kill_handler(int signum);
void * runner(void * params);

int main() {
  struct word Words[SIZE];
  char c = 'y';
  for(real_size = 0; ; real_size++) {
    if(add_word(Words) < 0){
      fprintf(stderr, "Dictionary Space Exhausted\nNo new words can be added\n...\n");
      break;
    }
  }
  print_dict(Words);
  pthread_t tid;
  struct block args;
  args.Words = Words;
  args.l = 0;
  args.h = real_size - 1;
  args.other_tid = NULL;
  while(1) {
    found = 0;
    printf("Enter key to be searched: ");
    fflush(stdout);
    scanf("%s", args.keyword);
    pthread_create(&tid, NULL, runner, &args);
    pthread_join(tid, NULL);
    if(!found)
      printf("`~~~[X] The word '%s' not found\n", args.keyword);
  }
  return 0;
} 

// prints the dictionary
void print_dict(struct word * Words) {
  printf("*********************The Dictionary********************* \n");
  for (int i = 0; i < real_size; i++)
    printf("W: %s, mean1: %s, mean2: %s\n", Words[i].self, Words[i].mean1, Words[i].mean2);
  printf("******************************************************** \n");
}

// adds a word in the dictionary
int add_word(struct word * Words) {
  if(real_size == SIZE)
    return -1;
  printf("Input new data in the format: <word><space><primary-meaning><space><secondary-meaning>\n: ");
  scanf("%s %s %s", Words[real_size].self, Words[real_size].mean1, Words[real_size].mean2);
  return 1;
}

// Thread routine which binary searches in one(left/right) part of the array
void* runner(void * params) {
  struct block * args = params;
  if (args->l > args->h)     // Base case
    pthread_exit(NULL);

  int mid = (args->l + args->h) / 2;
  if (strcmp(args->Words[mid].self, args->keyword) == 0) {
    printf("`~~~>%s means: %s\n `--->Another meaning: %s\n", args->Words[mid].self, args->Words[mid].mean1, args->Words[mid].mean2);
    found = 1;
    pthread_exit(NULL);
  }

  pthread_t tid1, tid2;
  struct block left, right;
  left.l = args->l;
  left.h = mid - 1;
  strcpy(left.keyword, args->keyword);
  left.other_tid = &tid2;
  left.Words = args->Words;

  right.l = mid + 1;
  right.h = args->h;
  strcpy(right.keyword, args->keyword);
  right.other_tid = &tid1;
  right.Words = args->Words;
  pthread_create(&tid1, NULL, runner, &left); // search 'key' in left half
  pthread_create(&tid2, NULL, runner, &right); // search 'key' in right half
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_exit(NULL);
}
