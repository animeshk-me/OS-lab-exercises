#include<stdio.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>

#define SIZE 30

// A single word info
struct word {
  char self[20];    // the word
  char mean1[20];   // the first meaning
  char mean2[20];   // the second meaning
};

// A package to pass data among threads
struct block {
  int l;               // left end of Words[]
  int h;               // right end of Words[]
  char keyword[20];      // keyword to be searched
};

struct word Words[SIZE]; // the dictionary
int real_size = 0; // the real size of the dictionary at current time
int found = 0; 
_Bool allow_producer = 1;
_Bool allow_consumer = 0;
int duplicate_check = 0;

pthread_mutex_t lock; 

void* runner_producer(void * params);
void* runner_consumer(void * params);
void* runner(void * params);

int add_word();
void kill_handler(int signum);
_Bool is_duplicate_entry(char * keyword);
void print_dict();

int main() {
  // Mutex variable intialization
  if (pthread_mutex_init(&lock, NULL) != 0) { 
    printf("\n[X]mutex init has failed...\n"); 
    exit(1); 
  }
  // Creating producer and consumer threads
  pthread_t tid_producer, tid_consumer;
  pthread_create(&tid_producer, NULL, runner_producer, NULL);
  pthread_create(&tid_consumer, NULL, runner_consumer, &tid_producer); 
  pthread_join(tid_producer, NULL);
  pthread_join(tid_consumer, NULL);
  // Destroying the mutex variable
  pthread_mutex_destroy(&lock); 
  return 0;
} 

// The producer thread routine which parallely adds content to the Dictionary
void* runner_producer(void * params) {
  for(real_size = 0; ; real_size++) {
    while(!allow_producer) {  // Busy waiting for consumer to signal
      // wait
    }
    pthread_mutex_lock(&lock);
    // Critical section starts
    if(add_word() < 0){
      fprintf(stderr, "Dictionary Space Exhausted\n  >>No new words can be added\n  >>...\n");
      break;
    }
    // Critical section ends
    pthread_mutex_unlock(&lock);

    allow_producer = !(allow_producer);
    allow_consumer = !(allow_consumer);
  }
  print_dict();
  pthread_exit(NULL);
}

// handles the signal SIGUSR1 to kill the producer thread by the consumer thread
void kill_handler(int signum) {
  pthread_exit(NULL);
}

// The consumer thread routine, which searches in the dictionary
void* runner_consumer(void * params) {
  pthread_t * tid_producer = params;
  signal(SIGUSR1, kill_handler);
  pthread_t tid;
  struct block args;
  args.l = 0;
  while(1) {
    while(!allow_consumer) { // Busy waiting for producer to signal
      // wait
    }
    found = 0;
    pthread_mutex_lock(&lock);
    // Critical Section starts
    args.h = real_size - 1;
    printf("\n[CONSUMER MODE]: Enter key to be searched(enter 'exit' to quit the search operation): ");
    fflush(stdout);
    scanf("%s", args.keyword);
    if(strcmp(args.keyword, "exit") == 0) {
      pthread_kill(*tid_producer, SIGUSR1); // kill the producer before terminating
      break;
    }
    pthread_create(&tid, NULL, runner, &args);
    pthread_join(tid, NULL);
    if(!found)
      printf("\n[CONSUMER MODE](error): [X] The word '%s' not found\n", args.keyword);
    // Critical section ends
    pthread_mutex_unlock(&lock);
    allow_consumer = !(allow_consumer);
    allow_producer = !(allow_producer);
  }
  pthread_exit(NULL);
}

// adds a word in the dictionary(from stdin)
int add_word() {
  if(real_size == SIZE)   // Dictionary space exhausted
    return -1;
  struct word new;
  printf("\n[PRODUCER MODE]: Input new data in the format: <word><space><primary-meaning><space><secondary-meaning>\n: ");
  scanf("%s %s %s", new.self, new.mean1, new.mean2);
  if(is_duplicate_entry(new.self))
    printf("[PRODUCER MODE](error): Duplicate entries not allowed.\n");
  else  // in case of new entry
    Words[real_size] = new;
  return 1;
}

// Returns 1 if the keyword already exists in our dictionary
_Bool is_duplicate_entry(char * keyword) {
  found = 0;
  duplicate_check = 1;
  struct block args;
  args.l = 0;
  args.h = real_size - 1;
  strcpy(args.keyword, keyword);
  pthread_t tid;
  pthread_create(&tid, NULL, runner, &args);
  pthread_join(tid, NULL);
  duplicate_check = 0;
  return found; // whether keyword found or not 
}

// Thread routine which implements an efficient binary search using multithreading
void* runner(void * params) {
  struct block * args = params;
  if(args->l > args->h)     // Base case
    pthread_exit(NULL);

  int mid = (args->l + args->h) / 2;
  if(strcmp(Words[mid].self, args->keyword) == 0) {
    if(!duplicate_check)
      printf("[CONSUMER MODE](output): `---> %s means: %s\n                         `---> Another meaning: %s\n", Words[mid].self, Words[mid].mean1, Words[mid].mean2);
    found = 1;
    pthread_exit(NULL);
  }

  // Setting arguments to pass to both the right and left threads
  pthread_t tid1, tid2;
  struct block left, right;
  left.l = args->l;
  left.h = mid - 1;
  strcpy(left.keyword, args->keyword);

  right.l = mid + 1;
  right.h = args->h;
  strcpy(right.keyword, args->keyword);
  pthread_create(&tid1, NULL, runner, &left); // search 'keyword' in left half
  pthread_create(&tid2, NULL, runner, &right); // search 'keyword' in right half
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_exit(NULL);
}

// prints the dictionary
void print_dict() {
  printf("*********************The Dictionary********************* \n");
  for (int i = 0; i < real_size; i++)
    printf("W: %s, mean1: %s, mean2: %s\n", Words[i].self, Words[i].mean1, Words[i].mean2);
  printf("******************************************************** \n");
}