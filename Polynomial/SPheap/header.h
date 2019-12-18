#ifndef HEADER_H
#define HEADER_H

#define MEMORY 256*1024*1024
#define ASL_MAX 56

struct node
{
int i;
float a;
struct node* prev;
struct node* next;
};

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


int get_polynomial(struct node** poly);							// Input polynpmial		
void division(struct node** a_poly, struct node** b_poly);				// Performs the division operation
void delete_list(struct node** header);							// Deletes the list to free up space

void init_SPheap();
void* alloc(int size);
void free_alloc(void* ptr);
void print_SPheap_data();


#endif
