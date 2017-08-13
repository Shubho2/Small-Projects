#include<stdio.h>
#include<stdlib.h>

struct Node
{
	int top;
	int size;
	char *a;
};
typedef struct Node p;

void push(p **stk2,int k)
{
	if ((*stk2)->top==(*stk2)->size-1){
	(*stk2)->size = 2*(*stk2)->size;
	(*stk2)->a=(char *)realloc((*stk2)->a,(*stk2)->size);	
	}
	(*stk2)->a[++(*stk2)->top]=k;
}

int pop(p **s)
{
	int k;
	if((*s)->top == -1)
	{
		printf("Underflow\n");
	
	}
	else
	{
		if((*s)->top+1 <= (*s)->size/4)
		{
			(*s)->size = (*s)->size/2;
			(*s)->a = (char*)realloc((*s)->a,(*s)->size);
		} 
		k = (*s)->a[(*s)->top--];
		return k;
	}
}


void enque(p **stk1,int k)
{	
	if ((*stk1)->top==(*stk1)->size-1){
	(*stk1)->size = 2*(*stk1)->size;
	(*stk1)->a=(char *)realloc((*stk1)->a,(*stk1)->size);	
	}
	(*stk1)->a[++(*stk1)->top]=k;
}
void deque(p **stk1,p** stk2)
{
	if((*stk2)->top==-1)
	{
		if((*stk1)->top==-1)
		printf("Underflow\n");
		else
		{
		 	while((*stk1)->top !=-1)
		 	push(stk2,pop(stk1));
		 	printf("%d\n",pop(stk2));
		}
	}
	else
     	printf("%d\n",pop(stk2));
} 
  
void createNode(p **s)
{
	p *temp = (p*)malloc(sizeof(p));
	temp->a = (char*)malloc(sizeof(char));
	temp->top = -1;
	temp->size = 1;
	*s = temp;
}  		
			
	
		
int main()
{
	int n,k;
	p *stk1 = NULL;
	p *stk2 = NULL;
	createNode(&stk1);
	createNode(&stk2);
	while (1)
	{
		printf("1:enque\n2:deque\n3:exit\n");
		scanf("%d",&n);
		switch(n)
		{
			case 1: printf ("enter The element : ");
				scanf("%d",&k);
				enque(&stk1,k);
				break;
				
			case 2: deque(&stk1,&stk2);
        			break;
        			
			case 3: free(stk1->a);
				free(stk2->a);
				free(stk1);
				free(stk2);
				return 0;
			
		}
	}
}
