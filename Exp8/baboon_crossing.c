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

sem_t mutex, red_mutex, blue_mutex;
int count = 0;

void* runner1(void * params);
void* runner2(void * params);
void* runner_red_baboon(void * params);
void* runner_blue_baboon(void * params);
void *runner_terminator(void * params);
void kill_handler(int signum);

int main() {
  pthread_t tid[3];
  sem_init(&mutex, 0, 1);
  sem_init(&red_mutex, 0, MAX);
  sem_init(&blue_mutex, 0, MAX);
  pthread_create(&tid[0], NULL, runner1, NULL);
  pthread_create(&tid[1], NULL, runner2, NULL);
  pthread_create(&tid[2], NULL, runner_terminator, &tid);
  pthread_join(tid[2], NULL);
  return 0;
}

// the thread routine sync among the red baboons
void* runner1(void * params) {
  srand(time(0));
  while(1) {
    int count = rand() % 8; 
    sem_wait(&mutex);
    printf("This time %d Red baboons will cross\n", count);
    pthread_t tid[count];
    int num[count];
    for(int i = 0; i < count; i++) {
      num[i] = i + 1;
      pthread_create(&tid[i], NULL, runner_red_baboon, &num[i]);
    }
    for(int i = 0; i < count; i++) 
      pthread_join(tid[i], NULL);
    sem_post(&mutex);
    sleep(1);
  }
}

// the thread routine to sync among blue baboons
void* runner2(void * params) {
  srand(time(0));
  while(1) {
    int count = rand() % 8; 
    sem_wait(&mutex);
    printf("This time %d Blue baboons will cross\n", count);
    pthread_t tid[count];
    int num[count];
    for(int i = 0; i < count; i++) {
      num[i] = i + 1;
      pthread_create(&tid[i], NULL, runner_blue_baboon, &num[i]);
    }
    for(int i = 0; i < count; i++) 
      pthread_join(tid[i], NULL);
    sem_post(&mutex);
    sleep(1);
  }
}

// the thread routine to simulate a single red baboon
void* runner_red_baboon(void * params) {
  int * self = params;
  sem_wait(&red_mutex);
  printf("Red baboon%d is crossing the rope\n", *self);
  sleep(3);
  printf("Red baboon%d completed crossing the rope(3 sec)\n", *self);
  sem_post(&red_mutex);
  pthread_exit(NULL);
}

// the thread routine to simulate a single blue baboon
void* runner_blue_baboon(void * params) {
  int * self = params;
  sem_wait(&blue_mutex);
  printf("Blue baboon%d is crossing the rope\n", *self);
  sleep(3);
  printf("Blue baboon%d completed crossing the rope(3 sec)\n", *self);
  sem_post(&blue_mutex);
  pthread_exit(NULL);
}

// the thread which will terminate all threads after some time
void *runner_terminator(void * params) {
  pthread_t * tid_arr = params;
  signal(SIGUSR1, kill_handler);
  sleep(20);
  pthread_kill(tid_arr[0], SIGUSR1);
  pthread_kill(tid_arr[1], SIGUSR1);
  printf("******** All the threads killed after 20 secs. ****** \n");
  pthread_exit(NULL);
}

void kill_handler(int signum) {
  pthread_exit(NULL);
}