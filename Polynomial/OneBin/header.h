#ifndef HEADER_H
#define HEADER_H

#define MEMORY 256*1024*1024

struct node
{
int i;
float a;
struct node* prev;
struct node* next;
};

typedef struct binNode
{
	void* ptr;
	int address;
	struct binNode* next;
} binNode;



int get_polynomial(struct node** poly);							// Input polynpmial		
void division(struct node** a_poly, struct node** b_poly);				// Performs the division operation
void delete_list(struct node** header);							// Deletes the list to free up space

void init_bin(int size);
void print_bin_data();

#endif
