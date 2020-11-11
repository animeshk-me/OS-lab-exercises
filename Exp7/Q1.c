#include<stdio.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include<time.h>
#include<pthread.h>
#include<signal.h>
#include<stdlib.h>

#define BUFF_SIZE 5

int Buffer[BUFF_SIZE];
int in_ptr = 0;
int out_ptr = 0;

void* runner_producer(void * params);
void* runner_consumer(void * params);
void* runner_controller(void * params);

void kill_handler(int signum);
int ProduceItem();
void PrintBuffer();


int main() {
  pthread_t tids[3];
  pthread_create(&tids[0], NULL, runner_producer, NULL);
  pthread_create(&tids[1], NULL, runner_consumer, NULL);
  pthread_create(&tids[2], NULL, runner_controller, &tids);
  pthread_join(tids[2], NULL);
  return 0;
}

// the producer thread
void* runner_producer(void * params) {
  while(1) { 
    int new_item = ProduceItem();
    sleep(1);
    while((in_ptr + 1) % BUFF_SIZE == out_ptr) { // buffer full
      // do nothing
    }
    Buffer[in_ptr] = new_item;
    printf("[PRODUCER MODE]: produced data: %d\n", Buffer[in_ptr]);
    PrintBuffer();
    in_ptr = (in_ptr + 1) % BUFF_SIZE;
  }
  pthread_exit(NULL);
}

// The consumer thread
void* runner_consumer(void * params) {
  while(1) {
    while(in_ptr == out_ptr) {  // buffer empty
      // do nothing
    }
    sleep(5);
    printf("[CONSUMER MODE]: consumed data: %d\n", Buffer[out_ptr]);
    out_ptr = (out_ptr + 1) % BUFF_SIZE;
  }
  pthread_exit(NULL);
}

// Controls the two threads and terminates them
void* runner_controller(void * params) {
  pthread_t * tids = params;
  signal(SIGUSR1, kill_handler);
  sleep(60);
  pthread_kill(tids[0], SIGUSR1);
  pthread_kill(tids[1], SIGUSR1);
  pthread_join(tids[0], NULL);
  pthread_join(tids[1], NULL);
  printf("Controller Thread terminates both Producer and consumer after 60 seconds of execution.\n");
  pthread_exit(NULL);
}

// Kills the thread when SIGUSR1 is recieved
void kill_handler(int signum) {
  pthread_exit(NULL);
}

// produces a new item randomly and return
int ProduceItem() {
  srand(time(0));
  return ((rand() % 10000) + 100);
}

// Prints the current status of the buffer
void PrintBuffer() {
  printf("The buffer status: ");
  fflush(stdout);
  for(int i = 0; i < BUFF_SIZE; i++)
    printf("%d ", Buffer[i]);
  printf("\n");
}