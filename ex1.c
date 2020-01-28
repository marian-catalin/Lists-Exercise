#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "API.h"

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;

node *ListHead = NULL;

void main(){

    printf("Starting program...\n");

    init_barrier();
    init_mutex();

    printf("Creating threads...\n");

    if(pthread_create(&thread1, NULL, Thread1_Function, NULL) != 0){
      printf("Creating first thread failed!\n");
      return;
    };

    if(pthread_create(&thread2, NULL, Thread2_Function, NULL) != 0){
      printf("Creating second thread failed!\n");
      return;
    };

    if(pthread_create(&thread3, NULL, Thread3_Function, NULL) != 0){
      printf("Creating third thread failed!\n");
      return;
    };

    printf("Joining Threads...\n");

    if(pthread_join(thread1, NULL) != 0){
      printf("Error joining first thread!\n");
      return;
    };

    if(pthread_join(thread2, NULL) != 0){
      printf("Error joining second thread!\n");
      return;
    };

    if(pthread_join(thread3, NULL) != 0){
      printf("Error joining third thread!\n");
      return;
    };

    printf("Final state of the list after threads execution:\n");
    node *current = ListHead;
    while(current != NULL) {
      current->print(current);
      current = current->next;
    }
    if(current == NULL)
      printf("End of list!\n");

    sleep(2);

    printf("Flushing list...\n");
    flush_list(&ListHead);

    printf("End of program!\n");

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&lock);
};

/*
Research:
https://timmurphy.org/2010/05/04/pthreads-in-c-a-minimal-working-example/
https://stackoverflow.com/questions/19704878/interlinked-struct-and-callback-in-c
https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/
https://www.geeksforgeeks.org/linked-list-set-3-deleting-node/
https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
*/
