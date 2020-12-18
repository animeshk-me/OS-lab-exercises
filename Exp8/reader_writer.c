#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<signal.h>
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>
#include<semaphore.h>

#define MAX 5

// A structure to pack the data to the threads
struct block {
  pthread_t * writers;
  pthread_t * readers;
};

sem_t reader_mutex, room_empty;
int num_readers = 0;

int data_block = 0; // the shared data_block


void* runner_terminator(void * params);
void* runner_writer(void * params);
void* runner_reader(void * params);
void kill_handler(int signum);

int main() {
  int nums[] = {0, 1, 2, 3, 4};
  sem_init(&reader_mutex, 0, 1);
  sem_init(&room_empty, 0, 1);
  pthread_t writers_tid[MAX];
  pthread_t readers_tid[MAX];
  pthread_t tid;
  struct block args;
  for (int i = 0; i < MAX; i++) {
    pthread_create(&writers_tid[i], NULL, runner_writer, &nums[i]);
    pthread_create(&readers_tid[i], NULL, runner_reader, &nums[i]);
  }
  args.writers = writers_tid;
  args.readers = readers_tid;
  pthread_create(&tid, NULL, runner_terminator, &args);
  pthread_join(tid, NULL);
  return 0;
}

// the thread routine for writers
void* runner_writer(void * params) {
  int * self = params;
  while(1) {
    sem_wait(&room_empty);
    // Critical section starts
    printf("Writer%d in the CS\n", *self);
    data_block++;
    sleep(0);
    printf("Writer%d writes %d\n", *self, data_block);
    printf("Writer%d exits the CS\n...\n", *self);
    // Critical section ends
    sem_post(&room_empty);
    sleep(1);
  }
}

// the thread routine for readers
void* runner_reader(void * params) {
  int * self = params;
  while(1) {
    sem_wait(&reader_mutex);
    num_readers++;
    if (num_readers == 1)
      sem_wait(&room_empty);
    sem_post(&reader_mutex);
    
    // Critical section starts
    printf("Reader%d in the CS\n", *self);
    printf("Reader%d reads %d\n", *self, data_block);
    sleep(0);
    printf("Reader%d exits the CS\n...\n", *self);
    // Critical section ends

    sem_wait(&reader_mutex);
    num_readers--;
    if (num_readers == 0)
      sem_post(&room_empty);
    sem_post(&reader_mutex);
    sleep(1);
  }
}

// the terminator which will terminate all the threads
void *runner_terminator(void * params) {
  struct block * args = params;
  signal(SIGUSR1, kill_handler);
  sleep(5);
  for (int i = 0; i < MAX; i++) {
    pthread_kill(args->readers[i], SIGUSR1);
    pthread_kill(args->writers[i], SIGUSR1);
  }
  printf("All readers and writers killed after 5 seconds.\n");
  pthread_exit(NULL);
}

void kill_handler(int signum) {
  pthread_exit(NULL);
}