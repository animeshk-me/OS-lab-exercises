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
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// The philosopher information
struct phil {
  int self; // the current philosopher number
  int left; // the left philosopher number
  int right; // the right philosopher number
  int state;  // thinking/hungry/eating
};

struct phil Philosophers[MAX];
sem_t overall_mutex; // to control the overall synchronisation
sem_t spoon_sem[MAX];

void* runner_philosopher(void *params);
void init_phil();
void take_fork(int num);
void put_fork(int num);
void kill_handler(int signum);
void test(int num);

  
int main() {
  pthread_t tid[MAX];
  signal(SIGUSR1, kill_handler);
  sem_init(&overall_mutex, 0, 1);
  init_phil();
  for (int i = 0; i < MAX; i++)
    sem_init(&spoon_sem[i], 0, 0);
  for (int i = 0; i < MAX; i++) {
    pthread_create(&tid[i], NULL, runner_philosopher, &Philosophers[i].self);
    printf("Philosopher %d is thinking.\n", i);
  }
  sleep(25);
  for (int i = 0; i < MAX; i++){
    pthread_kill(tid[i], SIGUSR1);
    printf("[X][X][X]The main thread kills the philosopher %d.\n", i);
  }
  printf("Exiting...\n");
  // pthread_join(tid[i], NULL);
  return 0;
} 

void kill_handler(int signum) {
  pthread_exit(NULL);
}


// The runner thread to implement the running philosopher
void *runner_philosopher(void * params) {
  int * num = params;
  while(1) {
    sleep(1);
    take_fork(*num);
    sleep(0);       // to force the context switch to happen
    put_fork(*num);
  }
  pthread_exit(NULL);
}

// initializes the philosopher numbers
void init_phil() {
  for (int i = 0; i < MAX; i++) {
    Philosophers[i].self = i;
    Philosophers[i].left = (i + 1) % MAX;
    Philosophers[i].right = (i + (MAX - 1)) % MAX;
    Philosophers[i].state = THINKING;
  }
}

// test if it is possible to take the fork
void test(int num) {
  if(Philosophers[num].state == HUNGRY && Philosophers[Philosophers[num].left].state != EATING && Philosophers[Philosophers[num].right].state != EATING) {
    Philosophers[num].state = EATING;
    sleep(2);
    printf("Philosopher %d takes fork %d and %d\n", num, Philosophers[num].left, num);
    sem_post(&spoon_sem[num]);
  }
}

// Take the fork after testing
void take_fork(int num) {
  sem_wait(&overall_mutex);
  Philosophers[num].state = HUNGRY;
  printf("Philosopher %d is hungry\n", num);
  test(num);
  sem_post(&overall_mutex);
  sem_wait(&spoon_sem[num]);
  sleep(1);
}

// put the fork down
void put_fork(int num) {
  sem_wait(&overall_mutex);
  Philosophers[num].state = THINKING;
  printf("Philosopher %d puts fork %d and %d\n", num, Philosophers[num].left, num);
  test(Philosophers[num].left);
  test(Philosophers[num].right);
  sem_post(&overall_mutex);
}


