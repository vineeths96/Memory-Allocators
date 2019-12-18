#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"

int bin_size;
int max_num_bins, total_request_size;
int num_alloc_bin, bin_alloc_request, bin_dealloc_request;

binNode* alloc_bin_list_head;
binNode* deleted_bin_list_head;
//char* bin];
void *bin;
double time_alloc, time_free;
clock_t beg, end; 

// Initialize bin
void init_bin(int size)
{
  bin_size=size;
  bin = malloc(MEMORY);

  max_num_bins=(MEMORY/size);

  alloc_bin_list_head=NULL;
  deleted_bin_list_head=NULL;

  num_alloc_bin=0;
  bin_alloc_request=0;
  bin_dealloc_request=0;
  total_request_size=0;
  time_alloc = 0;
  time_free = 0;
}


void* alloc_bin(int size)
{
  static int ind=0;
  binNode* headNode;
  
  bin_alloc_request++;
  total_request_size += size;

  if(num_alloc_bin >= max_num_bins)
    {
      printf("Whole memory is allocated. Free some memory to use\n");
      return NULL;
    }
  
  if(size>bin_size)
    {
      printf("Requested memory greater than bin size. Restart the program with larger bin size\n");
      return NULL;
    } 

  // Using deleted bins if available
  if(deleted_bin_list_head != NULL)
    {
      // Appeding the first element to alloc list
      headNode=deleted_bin_list_head;
      deleted_bin_list_head=deleted_bin_list_head->next;
      headNode->next=alloc_bin_list_head;
      alloc_bin_list_head=headNode;
  
      num_alloc_bin++;
      
      return alloc_bin_list_head->ptr;	
    }
  else
    {
      binNode* newnode=(binNode*)malloc(sizeof(binNode));
      
      newnode->next=alloc_bin_list_head;
      alloc_bin_list_head=newnode;

      // Addressing in bin
      newnode->address=ind;
      newnode->ptr=&bin[ind];
      ind=ind+bin_size;
      
      num_alloc_bin++;
      
      return newnode->ptr;
    }
}

void free_bin(struct node* delNode)
{
  void* bin_ptr=(void*)delNode;
  binNode* lCrawl;
  binNode *prev;
  
  bin_dealloc_request++;

  // Checking the allocated list for pointer
  for(lCrawl=alloc_bin_list_head; lCrawl!=NULL; lCrawl=lCrawl->next)
    {
      if(lCrawl->ptr == bin_ptr) break;
      else prev = lCrawl;
    }

  if(lCrawl == NULL)
    {
      printf("Bin does not exist\n");
    }
  else
    {
      // Deleting the node from allocated list
      if(lCrawl == alloc_bin_list_head)
	alloc_bin_list_head=lCrawl->next;
      else
	prev->next=lCrawl->next;

      // Appending in deleted list
      lCrawl->next=deleted_bin_list_head;
      deleted_bin_list_head=lCrawl;
      
      num_alloc_bin--;
    } 
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
	struct node* new_node = (struct node*) alloc_bin(sizeof(struct node));
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


float find(struct node* header, int index)
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
	struct node* ptr
	  ;
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
		free_bin(prev);
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
		q = (find(*a_poly, (b_deg + k)))/find(*b_poly, b_deg);
		add_last(&quotient, k, q);

		for(int j = (b_deg + k -1); j >= k; j--)
		{
			p = find(*a_poly, j) - q * find(*b_poly, (j-k)); 
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


void print_bin_data()
{
  float int_frag = (float) ((bin_alloc_request*bin_size)-total_request_size)/(total_request_size);
  printf("\nNumber of allocated bins : %d",num_alloc_bin);	
  printf("\nNumber of bin allocation request : %d",bin_alloc_request);
  printf("\nNumber of bin deallocation request : %d",bin_dealloc_request);
  printf("\nInternal fragmentation : %f",int_frag);
  printf("\nTotal time taken for allocation : %lf",time_alloc);
  printf("\nTotal time taken for free : %lf",time_free);	
}

