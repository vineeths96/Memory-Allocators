#include <stdio.h>
#include "header.h"

int main()
{
  int bin_size;
  char ch;
  printf("Enter the bin size to work with (min 24)\n");
  scanf("%d", &bin_size);
  init_bin(bin_size);

  do
    {	
      // Declarations
      int flag = 0;
      struct node* a_poly = NULL;
      struct node* b_poly = NULL;

      // Polynomial inputs
      printf("\n*****************************************************************************");
      printf("\nEnter the coefficients as comma seperated value pair of format (power, coefficient)\n");

      flag = get_polynomial(&a_poly);
      if(flag)	break;
      flag = get_polynomial(&b_poly);
      if(flag)  break;
		
      // Division
      division(&a_poly, &b_poly);

      // Deleting lists to free up space
      delete_list(&a_poly);
      delete_list(&b_poly);

      printf("Do you wish to continue (Y or N)\n");
      scanf(" %c", &ch);
      
    }while(ch!= 'N' && ch!= 'n');	

  print_bin_data();
  printf("\n");
}
