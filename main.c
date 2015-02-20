#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CHAIRS 3

void * student(void *param);
void * teacher(void *param);

enum student_state {
  PROGRAMMING,
  WAITING,
  BEING_HELPED
};

enum teacher_state {
  HELPING,
  SLEEPING
};

int run = 1;
sem_t teacher_sem;
sem_t student_sem;

void * student(void *param) {
  enum student_state state = PROGRAMMING;
  // case the param to an int;
  int student_number = *((int*)param);
  while(run) {
    switch(student_state) {
      case PROGRAMMING:
        // 1.) Program for a period of time
        // wait for 0 - 1000 ms, in increments of 100ms
        nanosleep((rand() % 10) * 100000000);
        state = WAITING;
        break;
      case WAITING:
        // post to the teacher's sem, to alert that we want help
        sem_post(&teacher_sem);
        // wait for the teacher to give back the sem after finished helping
        sem_wait(&student_sem);
        state = PROGRAMMING;
        break;
      case BEING_HELPED:
        state = PROGRAMMING;
        break;
    }

    // just as a test spew 
    printf("student %d says hi\n", student_number);
    sleep(1);
  }
  return NULL;
}

void * teacher(void *param) {
  while(run) {
    // 1.) When no students waiting, nap
    sem_wait(&teacher_sem);
    printf("TA is helping a student\n");
    sem_post(&student_sem);
    // 2.) When students waiting, help the student
    printf("TA says hi\n");
    sleep(1);
  }
  return NULL;
}

int main(int argc, char * argv[]) { 
  if(argc != 2) {
    printf("Usage: %s student_number\n", argv[0]);
    exit(1);
  }
  int student_number = atoi(argv[1]);

  // initialize the random number generator
  srand(time(NULL));

  // initialize the teacher signaling semaphor
  sem_init(&teacher_sem, 0, 1);
  sem_init(&student_sem, 0, num_students);

  // teacher thread and attributes
  pthread_t teacher_thread;
  pthread_attr_t teacher_attr;

  // student threads and attributes, and number
  pthread_t students_thread[student_number];
  pthread_attr_t students_attr[student_number];
  int students_id[student_number];

  // initialize the teacher thread
  pthread_attr_init(&teacher_attr);
  pthread_create(&teacher_thread, &teacher_attr, teacher, NULL);

  // initialize the student threads
  int i;
  for(i = 0; i < student_number; i++) {
    pthread_attr_init(&students_attr[i]);
    students_id[i] = i;
    pthread_create(&students_thread[i], &students_attr[i], student, &students_id[i]);
  }

  // let the threads run
  sleep(10);
  // then tell them to pack up close
  run = 0;
  
  // join all the threads back to the main thread
  pthread_join(teacher_thread, NULL);
  for(i = 0; i < student_number; i++) {
    pthread_join(students_thread[i], NULL);
  }

  // successful execution!
  return 0;
}
