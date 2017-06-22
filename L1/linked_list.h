#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 81

// khai bao 

typedef struct elemt {
  char name[MAX];
  int loop ;
  int line[MAX];
} elemt;

typedef struct node {
  elemt data;
  struct node *next;
} node;


node *makeNewNode(elemt e);
int isEmpty(node *root);
node *insert(node **root, elemt e);
int search(node *root, elemt e);
node *editLoop(node **root, elemt e,int x);
void printNode(node *p);
void printList(node *root);
void freeList(node **root);

node *makeNewNode(elemt e)
{
  node *new;
  new = (node *)malloc(sizeof(node));
  new->data = e;
  new->next = NULL;
}

int isEmpty(node *root)
{
  return !root;
}

// insert

node *insert(node **root, elemt e)
{
  node *new = makeNewNode(e);
  if(*root == NULL)
    {
      new->next = NULL;
      *root = new;
      return new;  
    }

  node *tmp = *root;
 
  while (strcmp(e.name,tmp->data.name) > 0)
    {
      if (tmp->next != NULL) tmp = tmp->next;
      else
        {
          new->next = NULL;
          tmp ->next = new;
          return new;
        }
    }
  if(tmp == *root)
    {
      new->next = tmp;
      *root = new;
      return new;
    }
  node *tmp2 = *root;
  
  for(;tmp2->next != tmp; tmp2 = tmp2->next);
  tmp2->next = new;
  new->next = tmp;
  return new;
}

// tim kiem ten trung

int search(node *root, elemt e)
{
  if(root == NULL)
    return 0;
  if(root->next == NULL && (strcmp(root->data.name,e.name) == 0))
    return 1;
  node *tmp = root;
  for(; tmp != NULL; tmp = tmp->next)
    if((strcmp(tmp->data.name,e.name) == 0))
      return 1;
    return 0;
  }

  // chinh sua thong so

  node *editLoop(node **root, elemt e,int x)
  {
    if(root == NULL)
      return NULL;
    node *tmp = *root;
    for(; tmp->next != NULL; tmp = tmp->next)
      if((strcmp(tmp->data.name,e.name) == 0))
	{
	  tmp->data.loop++;
	  int i;
	  while(tmp->data.line[i] != 0)
	    {
	      if(tmp->data.line[i] == x)
		break;
	      else
		i++;
	    }
	  tmp->data.line[i] = x;
	}

    return *root;

  }

  // in

  void printNode(node *p)
  {
    if(p == NULL)
      printf("No node!\n");
    else
      {
	printf("%s %d ",p->data.name,p->data.loop);
	int i = 0;
	while(p->data.line[i] != 0)
	  {
	    printf("%d ",p->data.line[i]);
	    i++;
	  }
	printf("\n");
      }
  }

  void printList(node *root)
  {
    if(root == NULL)
      printf("No List!\n");
    else
      {
	node *p = root;
	for(; p != NULL; p = p->next)
	  printNode(p);
      }
    return;
  }

  void freeList(node **root)
  {
    node *to_free = *root;
    while(to_free != NULL)
      {
	*root = (*root)->next;
	free(to_free);
	to_free = *root;
      }
  
  }
