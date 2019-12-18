#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "header.h"

void dealloc(int ind,int address);

// Global variables
ASL_box ASL[ASL_MAX];
allocList alloc_SL;

// Information variables
int num_alloc_request, num_dealloc_request;
int num_splits, num_recomb, num_alloc;
double sum_request, sum_alloc, current_area;
double time_alloc, time_free;
clock_t beg, end; 
char* mem;

ASL_node* addNode(int start, int buddy_ind, int parent_ind, int type, int tag)
{
  ASL_node* newnode = (ASL_node*)malloc(sizeof(ASL_node));
  
  newnode->next = NULL;
  newnode->prev = NULL;
	
  newnode->start = start;
  newnode->buddy_ind = buddy_ind;
  newnode->parent_ind = parent_ind;
  newnode->type =  type;
  newnode->tag = tag;

  return newnode;
}


void init_SPheap()
{
  mem = malloc(MEMORY);
  
  // ASLth element is min
  ASL[ASL_MAX-1].size = 1;
  ASL[ASL_MAX-2].size = 2;

  for(int i=ASL_MAX-3; i>= 0; i=i-2)
    {
      if (i == ASL_MAX-3)
	ASL[i].size=3*ASL[i+2].size;
      else
	ASL[i].size=3*ASL[i+3].size;

      ASL[i-1].size=2*ASL[i+1].size;
    }

  for(int i=0; i<ASL_MAX; i++)
    {
      ASL[i].head = NULL;
      ASL[i].tail = NULL;

      if(i%2 == 0)
	ASL[i].KVAL = (int)(log(ASL[i].size)/log(2));
      else
	ASL[i].KVAL = (int)(log((ASL[i].size)/3)/log(2));
    }
	
  ASL_node* newnode = addNode(0, -1, -1, -1, 0);
  ASL[0].head = newnode;
  ASL[0].tail = newnode;

  (alloc_SL).head = NULL;
  (alloc_SL).tail = NULL;

  num_alloc_request = 0;
  num_dealloc_request = 0;
  num_alloc = 0;
  num_splits = 0;
  num_recomb = 0;
  sum_request = 0;
  sum_alloc = 0;
}

// Adds to an allocated list that maintains the ASL index, pointer, starting address
void add_alloc_SL(int ind,void* ptr,ASL_node* node)
{
  allocNode* newnode = (allocNode*)malloc(sizeof(allocNode));

  if(alloc_SL.head == NULL)
    {
      alloc_SL.head = newnode;
      alloc_SL.tail = newnode;
    }
  else
    {
      alloc_SL.tail->next = newnode;
      alloc_SL.tail = newnode;	
    }

  newnode->address = node->start;	
  newnode->ind = ind;
  newnode->next = NULL;	
  newnode->node_ptr = ptr;
}


int split_type_1(int ind,int size,ASL_node* node)
{
  int start = node->start;

  // Adding new block to list
  ASL_node* new_node_1 = addNode(start, (ind+4), ind, 3, 0);
  new_node_1->prev = ASL[ind+1].tail;
	
  if(ASL[ind+1].tail != NULL)
    ASL[ind+1].tail->next = new_node_1;
	
  if(ASL[ind+1].head == NULL)
    ASL[ind+1].head = new_node_1;
	
  ASL[ind+1].tail = new_node_1;

  // Adding new block to list
  struct ASL_node* new_node_2 = addNode((start+((ASL[ind].size)*3/4)), (ind+1), ind, 3, 0);
  new_node_2->prev=ASL[ind+4].tail;

  if(ASL[ind+4].tail != NULL)
    ASL[ind+4].tail->next = new_node_2;
	
  if(ASL[ind+4].head == NULL)
    ASL[ind+4].head = new_node_2;
	
  ASL[ind+4].tail = new_node_2;

  // Check with block to return
  if((ASL[ind+4].size) >= size)
    return(ind+4);
  else
    return(ind+1);
}

int split_type_2(int ind,int size, ASL_node* node)
{
  int start = node->start;

  // Adding new block to list
  ASL_node* new_node_1 = addNode(start, (ind+3), ind, 1, 0);
  new_node_1->prev = ASL[ind+1].tail;

  if(ASL[ind+1].tail != NULL)
    ASL[ind+1].tail->next=new_node_1;

  if(ASL[ind+1].head == NULL)
    ASL[ind+1].head=new_node_1;

  ASL[ind+1].tail=new_node_1;

  // Adding new block to list
  ASL_node* new_node_2 = addNode((start+((ASL[ind].size)*2/3)), (ind+1), ind, 2, 0);
  new_node_2->prev=ASL[ind+3].tail;
	
  if(ASL[ind+3].tail != NULL)
    ASL[ind+3].tail->next = new_node_2;

  if(ASL[ind+3].head == NULL)
    ASL[ind+3].head=new_node_2;

  ASL[ind+3].tail=new_node_2;

    // Check with block to return
  if((ASL[ind+3].size) >= size)
    return (ind+3);
  else
    return (ind+1);
}


void* alloc(int size)
{
  ASL_node* node;
  void* ptr=NULL;

  // Finding an index to start search
  int k=(int)ceil((log(size)/log(2)));
  int ind=(ASL_MAX)-(2*k)+1;

  num_alloc_request++;

  while(ind>=0)
    {	
      if(ASL[ind].head == NULL)		
	ind=ind-1;
      else
	{
	  node=ASL[ind].head;
	  while(node!=NULL)
	    {
	      // Check if the block is used or not
	      if(node->tag == 0)
		break;
	      else
		node=node->next;			
	    }

	  // If end of list move to higher block
	  if(node!=NULL)
	    break;
	  else
	    ind=ind-1;
	}
    }
	
  if((ind == -1) && ((ASL[0].head)->tag!=0))
    {	
      printf("Allocation Failure\n");
      float per_area=(current_area/(MEMORY));
      printf("Requested size %d, Allocation percentage %f", size, per_area);
      exit(0);
    }

  // Recursively find a block with size >= required size
  while(1)
    {
      if(ind%2 == 0)
	{
	  //2^k split 
	  if(((ASL[ind].size)/4) >= size || ((ASL[ind].size)*3/4) >= size)
	    {
	      // Returns smallest size >= required size
	      ind=split_type_1(ind,size,node);
	      num_splits++;
	      node->tag=1;
	      node=ASL[ind].head;

	      // Search ASL for that index
	      while(node!=NULL)
		{
		  if(node->tag == 0)
		    break;
		  else
		    node=node->next;			
		}
	    }
	  else
	    {
	      // If split is not possible allocate the whole block
	      ptr = &mem[node->start];
	      add_alloc_SL(ind,ptr,node);
	      node->tag=1;
	      num_alloc++;
	      sum_request=sum_request+size;
	      sum_alloc=sum_alloc+ASL[ind].size;
	      current_area=current_area+ASL[ind].size;

	      break;
	    }
	}
      else
	{
	  // 3*2^k split
	  if(((ASL[ind].size)/3) >= size || ((ASL[ind].size)*2/3) >= size)
	    {
	      // Returns smallest size >= required size
	      ind=split_type_2(ind,size,node);
	      num_splits++;
	      node->tag=1;
	      node=ASL[ind].head;

	      // Search ASL for that index
	      while(node!=NULL)
		{
		  if(node->tag == 0)
		    break;
		  else
		    node=node->next;			
		}
	    }
	  else
	    {
	      // If split is not possible allocate the whole block
	      ptr = &mem[node->start];
	      add_alloc_SL(ind,ptr,node);
	      node->tag=1;
	      num_alloc++;
	      sum_request=sum_request+size;
	      sum_alloc=sum_alloc+ASL[ind].size;
	      current_area=current_area+ASL[ind].size;

	      break;
	    }
	}
		
    }

  return(ptr);	
}

// Find ASL node with the address
ASL_node* find(int ind,int address)
{
  ASL_node* node = ASL[ind].head;
	
  while(node->start != address)
      node=node->next;

  return(node);
}


void merge(ASL_node* node_1, int ind_1,ASL_node* node_2,int ind_2)
{	
  num_recomb++;
	
  ASL_node* pNode;
  ASL_node* qNode;
  ASL_node* prev;

  // FInding parent node (bigger block has parent starting address)
  if(ASL[ind_1].size > ASL[ind_2].size)
    pNode=find(node_1->parent_ind, node_1->start);
  else
    pNode=find(node_2->parent_ind,node_2->start);

  //Setting it to available
  pNode->tag=0;
  int pNode_ind=node_1->parent_ind;

  // Remove from ASL the smaller chunk
  qNode=ASL[ind_1].head;
  while(qNode != node_1)
    {		
      prev=qNode;
      qNode=qNode->next;
    }
  
  if(qNode == ASL[ind_1].head)
    ASL[ind_1].head=qNode->next;
  else
    prev->next=qNode->next;

  if(qNode == ASL[ind_1].tail)
    ASL[ind_1].tail=qNode->prev;

  free(qNode);

  // Remove from ASL the smaller chunk
  qNode=ASL[ind_2].head;
  while(qNode != node_2)
    {		
      prev=qNode;
      qNode=qNode->next;
    }

  if(qNode == ASL[ind_2].head)
    ASL[ind_2].head=qNode->next;
  else
    prev->next = qNode->next;
  if(qNode == ASL[ind_2].tail)
    ASL[ind_2].tail=qNode->prev;	

  free(qNode);

  // Recursivley do it
  dealloc(pNode_ind, pNode->start);
}


void dealloc(int ind, int address)
{
  // Boundary cases
  if(ind != 0)
    {
      ASL_node* pNode;
      ASL_node* qNode;
      int buddy_address;

      // Find the node type
      pNode=find(ind,address);

      // Finding buddy address and merging
      if(pNode->type == 3)
	{
	  if((ASL[ind].KVAL) == (int)(log(ASL[ind].size)/log(2)))
	    buddy_address=(address-(3*(ASL[ind].size)));		
	  else if((ASL[ind].KVAL) == (int)(log(ASL[ind].size/3)/log(2)))
	    buddy_address=(address+(ASL[ind].size));
	  else
	    printf("\Error\n");
	}
      else if(pNode->type == 1)
	buddy_address=(address+(ASL[ind].size));
      else if(pNode->type == 2)
	buddy_address=(address-(2*(ASL[ind].size)));	

      // Buddy check
      qNode = find(pNode->buddy_ind, buddy_address);

      // Buddy merge
      if(qNode->tag == 0)
	merge(pNode, ind, qNode, pNode->buddy_ind);
    }
}


void free_alloc(void* ptr)
{
  int ind;
  allocNode* prev;
  allocNode* pNode;
  //allocNode* qNode;
  
  num_dealloc_request++;
  pNode = alloc_SL.head;

  // Find the node in alloc_SL
  while(pNode != NULL && pNode->node_ptr != ptr)
    {
      prev=pNode;
      pNode=pNode->next;		
    }
  
  // Current allocated area
  current_area=current_area-(ASL[pNode->ind].size);

  // Pointer found in alloc_SL
  if(pNode != NULL)
    {
      dealloc(pNode->ind, pNode->address);	
      num_alloc--;
    }
  else
      printf("Allocation does not exist");
  
  if(pNode == alloc_SL.head)
    alloc_SL.head=pNode->next;	
  else
    prev->next=pNode->next;
}



int add_last(struct node** header, int i, float a)
{
	// If coefficient is zero then return and iterate for next set of polynomials
	if(a == 0)	
	{
		printf("Coefficeint cannot be 0. Try again.\n");
		return 1;
	}

	beg = clock();
	struct node* new_node = (struct node*) alloc(sizeof(struct node));
	end = clock();

	if(!new_node) exit(0);
	
	time_alloc+=(double)(end-beg)/CLOCKS_PER_SEC;
	
	// If non zero add element to list
	new_node->a = a;
	new_node->i = i;
	new_node->next = NULL;

	if(*header == NULL)
	{	
		*header = new_node;
		new_node->prev = NULL;
	}
	else
	{
	  struct node* last;
		for(last = *header; last->next != NULL; last = last->next);
		last->next = new_node;
		new_node->prev = last;

		// If the order of power is not descending
		if(new_node->i != ((new_node->prev)->i - 1))
		{
			printf("Order of power in input is not descending\n");
			return 1;
		}
	}
	return 0;
}


float find_poly(struct node* header, int index)
{
	struct node* ptr = NULL;
	int i = header->i;

	// Find and return the coefficient value and the index
	for(ptr = header; ptr != NULL; ptr = ptr->next, i--)
	{
		if(i == index)	return ptr->a;
	}
}

void update(struct node** header, int index, float val)
{
	struct node* ptr = NULL;
	int i = (*header)->i;

	// Find and update the coefficient value and the index	
	for(ptr = *header; ptr != NULL; ptr = ptr->next, i--)
	{
		if(i == index)	ptr->a = val;
	}


}

void put_polynomial(struct node* header)
{
	struct node* ptr;
	
	if(header == NULL) printf("%d", 0);
	// Display the polynomial on screen
	for(ptr = header; ptr != NULL; ptr = ptr->next)
	{
		printf("(%d, %f)\n", ptr->i, ptr->a);
	}
}


void delete_list(struct node** header)
{
	struct node* prev = *header;

	// Delete the nodes of the list
	while (*header != NULL)
	{
		*header = (*header)->next;
		beg = clock();
		free_alloc(prev);
		end = clock();
		
		time_free+=(double)(end-beg)/CLOCKS_PER_SEC;
		
		prev = *header;
	}
}
		

int get_polynomial(struct node** poly)
{
	int i, degree = 0, flag = 0;
	float a;

	printf("\nEnter the degree of the polynomial\n");
	scanf("%d", &degree);

	printf("Enter the coefficients of polynomial on decreasing value of power\n");

	// Receiving polynomial in decreasing order of power
	for(int j = degree; j >= 0; j--)
	{	
		// Receiving comma seperated (power, coefficient) pairs
		scanf("%d,%f", &i, &a);

		// If degree and first input doesnt match
		if((j == degree) && (j != i))
		{
			printf("Degree and input power does not match. Try again.\n");
			return 1;
		}
	
		// If matches add the element to the back of the list
		flag = add_last(poly, i, a);
		if(flag)	return 1;
	}

}


void division(struct node** a_poly, struct node** b_poly)
{
	struct node* quotient = NULL;
	struct node* remainder = NULL;
	float p,q;
	
	int a_deg = (*a_poly)->i, b_deg = (*b_poly)->i;

	for(int k = (a_deg-b_deg); k >= 0; k--)
	{
		// Find quotient and add it to quotient list
		q = (find_poly(*a_poly, (b_deg + k)))/find_poly(*b_poly, b_deg);
		add_last(&quotient, k, q);

		for(int j = (b_deg + k -1); j >= k; j--)
		{
			p = find_poly(*a_poly, j) - q * find_poly(*b_poly, (j-k)); 
			update(a_poly, j, p);
		}		
	}
	
	struct node* ptr = NULL;
	int i = a_deg;

	// Assigning the remainder list
	for(ptr = *a_poly; ptr != NULL; ptr = ptr->next, i--)
	{
		if(i == (b_deg-1))	
		{	
			remainder = ptr;
			break;
		}
	}

	// Print the reuslt out	
	printf("\nQuotient : \n");
	put_polynomial(quotient);

	printf("\nRemainder : \n");
	put_polynomial(remainder);

	delete_list(&quotient);
	printf("\n\n");
}


void print_SPheap_data()
{
  float intr_frag=(float) (sum_alloc-sum_request)/sum_request;
  	printf("\nNumber of allocations : %d",num_alloc);
	printf("\nNumber of allocation requests : %d",num_alloc_request);
	printf("\nNumber of deallocation request : %d",num_dealloc_request);
	printf("\nNumber of area splits : %d",num_splits);
	printf("\nNumber of buddy recombination : %d",num_recomb);
	printf("\nInternal fragmentation : %f",intr_frag);
	printf("\nTotal time for allocation : %lf seconds",time_alloc);
	printf("\nTotal time for free : %lf seconds",time_free);
	printf("\n");
}

