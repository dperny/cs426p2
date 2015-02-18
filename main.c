#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int run = 1;

void * student(void *param);
void * teacher(void *param);

void * student(void *param) {
  // case the param to an int;
  int student_number = *((int*)param);
  while(run) {
    /*
    * 1.) Program for a period of time
    * 2.) Seek help from TA
    *   a.) if TA is sleeping, wake the TA
    *   b.) if the TA is helping another student, wait in a chair
    *     i.) if there are no chairs, leave and go back to programming
    */
    
    // just as a test spew 
    printf("student %d says hi\n", student_number);
    sleep(1);
  }
  return NULL;
}

void * teacher(void *param) {
  while(run) {
    // 1.) When no students waiting, nap
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
