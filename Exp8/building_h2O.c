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

void* runner_hydrogen1(void *params);
void* runner_hydrogen2(void *params);
void* runner_oxygen(void *params);
void* runner_handler(void *params);
void* runner_terminator(void *params);
void kill_handler(int signum);

int count = 0;

sem_t to_h1, from_h1;
sem_t to_h2, from_h2;
sem_t to_oxy, from_oxy;

int main() {
  pthread_t tid[5];
  sem_init(&to_h1, 0, 0);
  sem_init(&from_h1, 0, 0);
  sem_init(&to_h2, 0, 0);
  sem_init(&from_h2, 0, 0);
  sem_init(&to_oxy, 0, 0);
  sem_init(&from_oxy, 0, 0);

  void* (*runners[])(void *) = {runner_hydrogen1, runner_hydrogen2, runner_oxygen, runner_handler};
  for (int i = 0; i < 4; i++)
    pthread_create(&tid[i], NULL, *runners[i], NULL);
  pthread_create(&tid[4], NULL, runner_terminator, tid);
  pthread_join(tid[3], NULL);
  return 0;
}

// The thread routine for first hydrogen atom
void *runner_hydrogen1(void * params) {
  while(1) { // keeps generating hydrogen molecules forever
    printf("[%d] Hydrogen thread 1 produced 1 atom and waiting for molecule to form...\n", count);
    sem_post(&from_h1);
    sem_wait(&to_h1);
    sleep(1);
  }
  pthread_exit(NULL); 
}

// The thread routine for second hydrogen atom
void *runner_hydrogen2(void * params) {
  while(1) { // keeps generating hydrogen molecules forever
    printf("[%d] Hydrogen thread 2 produced 1 atom and waiting for molecule to form...\n", count);
    sem_post(&from_h2);
    sem_wait(&to_h2);
    sleep(1);
  }
  pthread_exit(NULL); 
}


// The thread routine for oxygen atom
void *runner_oxygen(void * params) {
  while(1) { // keeps generating hydrogen molecules forever
    printf("[%d] Oxygen thread produced 1 atom and waiting for molecule to form...\n", count);
    sem_post(&from_oxy);
    sem_wait(&to_oxy);
    sleep(1);
  }
  pthread_exit(NULL); 
}

// The handler which takes care of synchronising the 3 threads
void *runner_handler(void * params) {
  while(1) {
    sem_wait(&from_h1);
    sem_post(&to_h1);
    printf("[%d] Hydrogen thread 1 invokes bond\n", count);

    sem_wait(&from_h2);
    sem_post(&to_h2);
    printf("[%d] Hydrogen thread 2 invokes bond\n", count);
    
    sem_wait(&from_oxy);
    sem_post(&to_oxy);
    printf("[%d] Oxygen thread invokes bond\n", count);

    printf("Set number %d completed successfully.\n", count);
    count++;
    sleep(1);
  }
  pthread_exit(NULL);
}

// to terminate all the threads after some time
void *runner_terminator(void * params) {
  pthread_t * tid_arr = params;
  signal(SIGUSR1, kill_handler);
  sleep(9);
  pthread_kill(tid_arr[0], SIGUSR1);
  printf("[X][X][X] Hydrogen thread 1 knowingly killed by terminator\n");
  pthread_kill(tid_arr[1], SIGUSR1);
  printf("[X][X][X] Hydrogen thread 2 knowingly killed by terminator\n");
  pthread_kill(tid_arr[2], SIGUSR1);
  printf("[X][X][X] Oxygen thread knowingly killed by terminator\n");
  pthread_kill(tid_arr[3], SIGUSR1);
  pthread_exit(NULL);
}

void kill_handler(int signum) {
  pthread_exit(NULL);
}

