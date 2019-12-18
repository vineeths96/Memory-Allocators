#ifndef HEADER_H
#define HEADER_H

#define MEMORY 256*1024*1024
#define ASL_MAX 56

typedef struct ASL_node
{
  int start;
  int type;
  int tag;
  int buddy_ind;
  int parent_ind;
  struct ASL_node* next;
  struct ASL_node* prev;
} ASL_node;

typedef struct ASL_list_box
{
  int size;
  int KVAL;
  struct ASL_node* head;
  struct ASL_node* tail;
} ASL_box;

typedef struct allocNode
{
  void* node_ptr;
  int ind;
  int address;
  struct allocNode* next;
} allocNode;

typedef struct allocList
{
  struct allocNode* head;
  struct allocNode* tail;
} allocList; 

typedef struct requestNode
{
  void* node_ptr;
  int time;
  struct requestNode* next;
} requestNode;

void init_SPheap();
int exp_random();
requestNode* addReqNode(int time, void* ptr, requestNode* head);

void* alloc(int size);
void free_alloc(void* ptr);

void print_SPheap_data();

void print_alloc_list();
#endif

