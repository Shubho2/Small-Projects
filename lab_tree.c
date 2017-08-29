#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
int ar =0;

struct tree{
	struct tree *left;
	struct tree *right;
	struct tree* mid;
	struct que *q;
	};
	
struct que{
	 char **s;
	 int front;
	 int rear;
	 int size;
	 int capacity;
	};
	
	 
void enQueue(struct que *q, char *str)  // function to put strings in the queue(satelite data)
{
        int i;
	if(q->size == q->capacity)  // checking if the queue is full
	{
		q->capacity *=2;
		char **r = (char**)malloc(sizeof(char*)*q->capacity);
		char **p = r;
		*r = q->s[q->front++];
		for(i = q->front; i < (q->rear); i=(i+1)%q->size)
		{
			*r = q->s[q->front];
			*r++;
		}
		q->front = 0;
		q->rear = q->size;
		char **temp = q->s;
		q->s = p;
		free(temp);
	}
	 
	q->s[q->rear] = str;
	q->rear = (q->rear+1)%q->capacity;
	q->size++;
}
		
void deQueue(struct que *q)    // function to delete string from the queue(satelite data)
{
	int i;
	if(q->size == 0)
	{
		printf("queue is empty\n");
		return;
	}
	else if(q->size == q->capacity/4)  // checking if the size of the queue is 1/4 th of the total capacity and then reduce the 
	{                                                                         // capacity to half of the current
		q->capacity /= 2;
		char **r = (char**)malloc(sizeof(char*)*q->capacity);
		char **p = r;
		*r = q->s[q->front++];
		for(i = q->front; i < (q->rear); i=(i+1)%q->size)
		{
			*r = q->s[q->front];
			*r++;
		}
		q->front = 0;
		q->rear = q->size;
		char **temp = q->s;
		q->s = p;
		free(temp);
	}
	
	q->front = (q->front+1)%q->capacity;
	q->size--;
}		


void init(struct que *q)  // initializing a queue
{
	q->s = (char**)malloc(sizeof(char*));
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->capacity = 1;
}


struct tree *search(struct tree *root, char *s) // search in the tree
{
	while(*s!='\0')
	{
		if(*s == 'L')
		root = root->left;
		else if(*s == 'M')
		root = root->mid;
		else if(*s == 'R')
		root = root->right;
		s++;
	}
	return root;
}	


void show(struct que *q)
{
	int m = q->front;
	int n = q->size;
	while(n)
	{
		printf("%s\n",q->s[m]);
		m = (m+1)%q->capacity;
		n--;
	}
	
}
void swap(struct tree **root, struct tree *r,struct tree *temp) // swapping two nodes
{
	struct tree *a,*b,*c;
	
	a = (*root)->left;
	b = (*root)->right;
	c = (*root)->mid;
	(*root)->left = r->left;
	(*root)->right = r->right;
	(*root)->mid = r->mid;
	r->left = a;
	r->right = b;
	r->mid = c;
	if(temp->left == r)
	temp->left = *root;
	else if(temp->right == r)
	temp->right = *root;
	else
	temp->mid = *root;
	*root = r;
} 	
	

struct tree *initialize()  // initialize a node in the tree
{
	struct tree *root = (struct tree*)malloc(sizeof(struct tree));
	root->left = NULL;
	root->right = NULL;
	root->mid = NULL;
	root->q = (struct que*)malloc(sizeof(struct que));
	init (root->q);
	return root;
} 

void traverse(struct tree *root)  // print the nodes in left->(root)-> mid->right order 
{
	if(root == NULL)
	return;
	traverse(root->left);
	show(root->q);
	traverse(root->mid);
	traverse(root->right);
}


void insert(struct tree *node)
{
	node->left = initialize();
	node->mid = initialize();
	node->right = initialize();
}


void enqueue(struct tree *root, char *s)
{
	root = search(root,s);	
	enQueue(root->q,s); // "1123" is used, beacause I don't know how to generate time string
}

void dequeue(struct tree *root, char *s)
{
	root = search(root,s);
	deQueue(root->q);
}

void access(struct tree **root, char *s)
{
	struct tree *r,*temp;
	r = *root;
	while(*s!='\0')
	{
		if(*s == 'L')
		{
			temp = r;
			r = r->left;
		}
		else if(*s == 'M')
		{
			temp = r;
			r = r->mid;
		}
		else if(*s == 'R')
		{
			temp = r;
			r = r->right;
		}
		s++;
	}
	struct tree *a,*b,*c;
	
	a = (*root)->left;
	b = (*root)->right;
	c = (*root)->mid;
	(*root)->left = r->left;
	(*root)->right = r->right;
	(*root)->mid = r->mid;
	if(temp != *root)
	{
		r->left = a;
		r->right = b;
		r->mid = c;
		if(temp->left == r)
		temp->left = *root;
		else if(temp->right == r)
		temp->right = *root;
		else
		temp->mid = *root;
	}
	else
	{
		if(temp->left == r)
		{
			r->left = *root;
			r->mid = c;
			r->right = b;
		}
		else if(temp->right == r)
		{
			r->right = *root;
			r->mid = c;
			r->left = a;
		}
		else
		{
			r->mid = *root;
			r->left = a;
			r->right = b;
		}
	}
	*root = r;
	
	//swap(root,r,temp);
}

void makeFree(struct tree *r) // freeing all the memory related to the subtree rooted at node r
{
	if(r == NULL)
	return;
	makeFree(r->left);
	makeFree(r->mid);
	makeFree(r->right);
	free(r->q->s);
	free(r->q);
	free(r);
}

void destroy(struct tree *root, char *s)
{
	root = search(root,s);
	makeFree(root);
}



int main()
{
	struct tree *root = initialize();
	
	insert(root);
	
	
	enqueue(root,"");
	//enqueue(root,"");
	enqueue(root,"L");
	traverse(root);
	struct tree *temp = search(root,"L");
	insert(temp);
	enqueue(root,"LL");
	dequeue(root,"");
	traverse(root);
	temp = search(root,"LM");
	insert(temp);
	enqueue(root,"LMR");
	enqueue(root,"LMR");
	dequeue(root,"LL");
	enqueue(root,"LM");
	traverse(root);
	dequeue(root,"LMR");
	access(&root,"LM");
	traverse(root);
	dequeue(root,"LMR");
	traverse(root);
        destroy(root,"");
        
        /*char s[20];
        char ch;
        int i = 0;
        int n;
        struct tree *root;
        while(1)
        {
        	
        	printf("1.Initialize\n2.Insert\n3.Traverse\n4.enqueue\n5.dequeue\n6.access\n7.destroy\n8.exit\n");
        	printf("Enter your choice: ");
        	scanf("%d",&n);
        	getchar();
        	switch(n)
        	{
        		case 1: root = initialize();
        			break;
        		
        		case 2: printf("give the position to insert in terms of string: ");
        			while((ch=getchar())!='\n')
        				s[i++] = ch;
        				
        			s[i] =	'\0';
        			struct tree *temp = search(root,s);
        			insert(temp);
        			i=0;
        			break;
        			
        		case 3: traverse(root);
        			break;
        		
        		case 4: printf("give the position to enqueue in terms of string: ");	 
      				while((ch=getchar())!='\n')
        				s[i++] = ch;
        				
        			s[i] =	'\0';
      				enqueue(root,s);
      				i=0;
      				break;
      				
      			case 5: printf("give the position to dequeue in terms of string: ");
      				while((ch=getchar())!='\n')
        				s[i++] = ch;
        				
        			s[i] =	'\0';
      				dequeue(root,s);
      				break;
      				
      			case 6: printf("give the position to access in terms of string: ");
      				while((ch=getchar())!='\n')
        				s[i++] = ch;
        				
        			s[i] =	'\0';
      				access(&root,s);
      				break;
      				
      			case 7: printf("give the position to destroy in terms of string: ");
      				while((ch=getchar())!='\n')
        				s[i++] = ch;
        				
        			s[i] =	'\0';
      				destroy(root,s);
      				break;
      				
      			case 8: return 0;
      			}
      		}*/		

}
