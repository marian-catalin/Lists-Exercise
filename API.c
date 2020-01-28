#include <stdio.h>
#include <stdlib.h>
#include "API.h"

node *ListHead;

/* -- Sync section -- */
void init_mutex(){
  int err = pthread_mutex_init(&lock, NULL);
  if(err != 0){
    printf("Mutex initialization failed!\n");
    return;
    }
  else{
    printf("Mutex initialized!\n");
    return;
    }
}

void init_barrier(){
    int err = pthread_barrier_init(&barrier, NULL, 3);
    if(err != 0){
      printf("Barrier initialization failed!\n");
      return;
    }
    else{
      printf("Barrier initialized!\n");
      return;
    };
}
/* --- Sync section --- */

/* -- print_list section -- */
void print_node(node *NodeToPrint){
  if(NodeToPrint == NULL){
      printf("Node is NULL!\n");
      return;
  };
  printf("%d\n", NodeToPrint->val);
};

void print_list(node *head){
    pthread_mutex_lock(&lock);
    printf("print_list() function in execution...\n");
    if(head == NULL){
      printf("List not initialized!\n");
      pthread_mutex_unlock(&lock);
      return;
    }
    else{
      node *current = head;

      while(current != NULL) {
        current->print(current);
        current = current->next;
      }
      if(current == NULL)
        printf("End of list!\n");
      pthread_mutex_unlock(&lock);
    };
}
/* -- print_list section -- */

/* -- add_node function -- */
/* Given a reference to the head
   of a list and an int, appends a new node at the end  */
void add_node(node **head_ref, int new_data){
    pthread_mutex_lock(&lock);
    printf("add_node() function in execution...\n");
    node *new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
      printf("Failed to create new node!\n");
      pthread_mutex_unlock(&lock);
    }
    else{
    /* 4. If the Linked List is empty, then make the new node as head */
      if (*head_ref == NULL){
       printf("List is empty!\n");
       printf("Head added to list!\n");
       printf("New Head = %i\n", new_data);
       *head_ref = new_node;

       /* 2. put in the data  */
       new_node->val = new_data;
       /* 3. This new node is going to be the last node, so make next of
       it as NULL*/
       new_node->next = NULL;
       new_node->print = &print_node;

       pthread_mutex_unlock(&lock);
       return;
      }

      /* 1. allocate node */
      if(sizeof(new_data) == 0){
      printf("Please insert data to be placed in the new node!\n");
      pthread_mutex_unlock(&lock);
      return;
      }

      printf("New node created!\n");

      node *last = *head_ref;  /* used in step 5*/

      /* 2. put in the data  */
      new_node->val = new_data;
      printf("Data in the new node: %i\n", new_data);

      /* 3. This new node is going to be the last node, so make next of
      it as NULL*/
      new_node->next = NULL;
      new_node->print = &print_node;

      /* 5. Else traverse till the last node */
      while (last->next != NULL)
        last = last->next;

      /* 6. Change the next of last node */
      last->next = new_node;
      pthread_mutex_unlock(&lock);
    };
}
/* -- add_node function -- */

/* -- delete_node function -- */
/* Given a reference (pointer to pointer) to the head of a list
   and a key, deletes the first occurrence of key in linked list */
void delete_node(node **head, int val){
    pthread_mutex_lock(&lock);
    printf("delete_node() function in execution...\n");
    /* Store head node */
    if(*head == NULL){
      printf("List not initialized!\n");
      pthread_mutex_unlock(&lock);
      return;
    }
    if(sizeof(val) == 0){
      printf("Please insert what data to delete from the list!\n");
      pthread_mutex_unlock(&lock);
      return;
    }
    node* temp = *head, *prev;

    /* If head node itself has to be deleted */
    if (temp != NULL && temp->val == val){
        *head = temp->next;   /* Changed head */
        printf("Head with val = %i deleted!\n", temp->val);
        printf("New head->val = %i\n", temp->next->val);
        free(temp);
        pthread_mutex_unlock(&lock);
        return;
    }

    /* Search for the node to be deleted, keep track of the
     previous node as we need to change 'prev->next' */
    while (temp != NULL && temp->val != val){
        prev = temp;
        temp = temp->next;
    }

    /* If key was not present in linked list */
    if (temp == NULL){
      printf("Node with val = %i not included in the list!\n", val);
      pthread_mutex_unlock(&lock);
      return;
    }

    /* Unlink the node from linked list */
    prev->next = temp->next;
    printf("Node with val = %i deleted!\n", temp->val);
    free(temp);
    pthread_mutex_unlock(&lock);
}
/* -- delete_node function -- */

/* -- sort_list function -- */
/* function to swap data of two nodes a and b*/
void swap(node *a, node *b){
    if(a == NULL || b == NULL){
        printf("Nothing to swap with!\n");
        return;
    }
    int temp = a->val;
    a->val = b->val;
    b->val = temp;
}

/* Bubble sort the given linked list */
void sort_list(node *head){
    pthread_mutex_lock(&lock);
    printf("sort_list() function in execution...\n");
    if(head == NULL){
      printf("List not initialized!\n");
      pthread_mutex_unlock(&lock);
      return;
    }
    int swapped;
    node *ptr1;
    node *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr)
        {
            if (ptr1->val > ptr1->next->val)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while(swapped);
    printf("List sorted!\n");
    pthread_mutex_unlock(&lock);
}
/* -- sort_list function -- */

/* -- flush_list function -- */
/* Function to delete the entire linked list */
void flush_list(node **head){
    printf("flush_list() function in execution...\n");
    if(*head == NULL){
      printf("List not initialized!\n");
      return;
    }
   node *current_node = *head;
   node *next_node;

   while (current_node != NULL)
   {
       next_node = current_node->next;
       free(current_node);
       current_node = next_node;
   }

   /* deref head_ref to affect the real head back
      in the caller. */
   *head = NULL;
   printf("List flushed!\n");
}
/* -- flush_list function -- */

/* -- Thread functions -- */
void *Thread1_Function(void *param){
    int tid;
    tid = pthread_self();

    pthread_barrier_wait(&barrier);

    add_node(&ListHead, 2);
    printf("Thread1 ID = %i -> add_node(%i)\n", tid, 2);

    add_node(&ListHead, 4);
    printf("Thread1 ID = %i -> add_node(%i)\n", tid, 4);

    add_node(&ListHead, 10);
    printf("Thread1 ID = %i -> add_node(%i)\n", tid, 10);

    delete_node(&ListHead, 2);
    printf("Thread1 ID = %i -> delete_node(%i)\n", tid, 2);

    sort_list(ListHead);
    printf("Thread1 ID = %i -> sort_list()\n", tid);

    delete_node(&ListHead, 10);
    printf("Thread1 ID = %i -> delete_node(%i)\n", tid, 10);

    delete_node(&ListHead, 5);
    printf("Thread1 ID = %i -> delete_node(%i)\n", tid, 5);
};

void *Thread2_Function(void *param){
    int tid;
    tid = pthread_self();

    pthread_barrier_wait(&barrier);

    add_node(&ListHead, 11);
    printf("Thread2 ID = %i -> add_node(%i)\n", tid, 11);

    add_node(&ListHead, 1);
    printf("Thread2 ID = %i -> add_node(%i)\n", tid, 1);

    delete_node(&ListHead, 11);
    printf("Thread2 ID = %i -> delete_node(%i)\n", tid, 11);

    add_node(&ListHead, 8);
    printf("Thread2 ID = %i -> add_node(%i)\n", tid, 8);

    print_list(ListHead);
    printf("Thread2 ID = %i -> print_list()\n", tid);
};

void *Thread3_Function(void *param){
    int tid;
    tid = pthread_self();

    pthread_barrier_wait(&barrier);

    add_node(&ListHead, 30);
    printf("Thread3 ID = %i -> add_node(%i)\n", tid, 30);

    add_node(&ListHead, 25);
    printf("Thread3 ID = %i -> add_node(%i)\n", tid, 25);

    add_node(&ListHead, 100);
    printf("Thread3 ID = %i -> add_node(%i)\n", tid, 100);

    sort_list(ListHead);
    printf("Thread3 ID = %i -> sort_list()\n", tid);

    print_list(ListHead);
    printf("Thread3 ID = %i -> print_list()\n", tid);

    delete_node(&ListHead, 100);
    printf("Thread3 ID = %i -> delete_node(%i)\n", tid, 100);
};
/* -- Thread functions -- */
