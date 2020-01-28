#include <pthread.h>

typedef struct ListNode node;
typedef void (*callback_print_t)(node*);

struct ListNode{
  int val;
  struct ListNode *next;
  callback_print_t print;
};

pthread_mutex_t lock;
pthread_barrier_t barrier;

void add_node(node **head_ref, int new_data);
void delete_node(node **head, int val);
void print_list(node *head);
void sort_list(node *head);
void flush_list(node **head);

void init_mutex();
void init_barrier();

void *Thread1_Function(void *param);
void *Thread2_Function(void *param);
void *Thread3_Function(void *param);
