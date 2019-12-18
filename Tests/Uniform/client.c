#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "header.h"

void main()
{	
  init_SPheap();
  srand(time(0));

  void* ptr;

  // Request list
  requestNode* delloc_list_head = NULL;
  requestNode* prev;
  requestNode* lCrawl;
  int t=0, S=0, T=0 ,j=0;

	while(t<2000)
	{
	  // Size
	  S=(rand()%1900)+100;
	  //printf("\n %d ",S);

	  // Life time
	  T=(random()%2000)+1;
	  //printf(" %d ",T);

	  ptr=alloc(S);
	  requestNode* newnode = addReqNode(t+T, ptr, delloc_list_head);
	  delloc_list_head = newnode;
	  prev = delloc_list_head;

	  /*
	  for(lCrawl=delloc_list_head; lCrawl!= NULL; lCrawl=lCrawl->next)
	    {
	      printf("\n %p %d", lCrawl->node_ptr, lCrawl->time);
	    }
	  */

	  // CHeck list to delete
	  for(lCrawl=delloc_list_head; lCrawl!= NULL; lCrawl=lCrawl->next)
	    {
	      // Deletion
	      if((lCrawl->time) == t)
		{
		  free_alloc(lCrawl->node_ptr);

		  if(lCrawl == delloc_list_head)
		    {
		      delloc_list_head = lCrawl->next;
		      prev = lCrawl->next;
		    }
		  else				
		    prev->next=lCrawl->next;

		  free(lCrawl);
		}
	      else
		prev = lCrawl;
	    }
	 
	 
	  t=t+1;
	  /*print_alloc_list();
	  break;
	  */
	  if(t%500 ==0)
	    {
	      printf("\ntime %d",t);
	      print_SPheap_data();
	    }

	}
}



