#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linked_list.h"

#define MAX 81

int tolowerWord(char tmp[MAX])
{
  tmp[0] = tolower(tmp[0]);
  return 0;
}


int main()
{
  
  //doc file stopw.txt
  //--------------------------------------------------------------------//

  node *root_stopw = NULL;
  char *file_stopw = "stopw.txt";
  FILE *f_stopw = fopen(file_stopw,"r");

  char tmp[MAX];
 
  while(!feof(f_stopw))
    {
      if(fscanf(f_stopw,"%s",tmp) != EOF)
	{
	  elemt a;
	  strcpy(a.name,tmp);
	  a.loop = 0;
	  int z = 0;
	  for(z = 0; z < MAX; z ++)
	    a.line[z] = 0;
	  insert(&root_stopw,a);
	}
    }
  fclose(f_stopw);
  
  //cho du lieu trong vanban.txt vao list
  //--------------------------------------------------------------------//
  
  node *root_vanban = NULL;
  char *filein = "vanban.txt";
  FILE *fin = fopen(filein,"r");

  char word[MAX];
  strcpy(word,"");
  int pos = 0,i,num_line = 0;
  char line[MAX];
  
  while(!feof(fin))
    {
      if(fgets(line,MAX,fin) != NULL)
	{
	  i = 0;
	  int flag_space = 0;
	  int private_noun = 0;
      
	  while(line[i] != '\0')
	    {      
	      char c = line[i++];
      
	      if(isalpha(c) || (c == '-'))
		{
		  while(word[pos++] != '\0');
		  word[pos-1] = c;
		  word[pos] = '\0';
		  pos = 0;

		  if(flag_space == 1 && (isupper(word[0]) != 0))
		    private_noun = 1;
		  else
		    private_noun = 0;
		} 
	      else
		{
		  if(c == ' ')
		    flag_space = 1;
		  else
		    flag_space = 0;
		  elemt e;
		  tolowerWord(word);
		  strcpy(e.name,word);
		  e.loop = 0;

		  if(strcmp(word,"") != 0 && (search(root_stopw,e) == 0) && private_noun == 0)
		    {
		  
		      elemt input;
		      strcpy(input.name,word);
		      
		      if(search(root_vanban,input) == 0)
			{
			  input.loop = 1;
			  int z = 0;
			  for(z = 0; z < MAX; z ++)
			    input.line[z] = 0;
			  input.line[0] = num_line + 1;
			  insert(&root_vanban,input);
			}
		      else
			editLoop(&root_vanban,input,num_line+1);
		    }
		  strcpy(word,"");
		}
	    }
	  num_line++;
	}
    }
   
  // ghi file
  //--------------------------------------------------------------------//
  
  char *file_index = "index.txt";
  FILE   *f_index = fopen(file_index,"w");
  
  node *p = root_vanban;
  for(; p != NULL; p = p->next)
    {
      fprintf(f_index,"%s %d ",p->data.name,p->data.loop);
      i = 0;
      while(p->data.line[i] != 0)
	fprintf(f_index,"%d ",p->data.line[i++]);   
      fprintf(f_index,"\n");
    }
  
  // free

  freeList(&root_vanban);
  freeList(&root_stopw);
  return 0;
}
