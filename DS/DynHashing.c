#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<limits.h>

#define bucket_l 2


typedef struct node{
	char *s;
	struct node *next;
	struct node *prev;
}node;


typedef struct bucket{
	unsigned int lch;  // db
	unsigned int length;
	node *head;
	}bucket;
	
typedef struct hash{
	bucket **b;
	unsigned int depth;
}hash;
	

unsigned int hashval(char *s,int depth)
{
	int i;
	unsigned int l = strlen(s);
	unsigned int k = INT_MAX;
        unsigned int j = 0;
  	int m = pow(2,depth);
	for(i=0;i<l;i++)
	j += (int)s[i];
	k = (j % k);
	k = k%m;
	return k;	
}

node *createnode(char *str)
{
	node *temp = (node*)malloc(sizeof(node));
	temp->prev = temp->next = NULL;
	temp->s = str;
	return temp;
}

node *insertList(node *head, char *s)
{
	
	if(head ==NULL)
	return createnode(s);
	else
	{
		node *temp = createnode(s);
		temp->next = head;
		head->prev = temp;
		head = temp;
		return head;
	}
}

bucket *createbucket(int d)
{
	bucket *temp = (bucket*)malloc(sizeof(bucket));
	temp->head = NULL;
	temp->length = 0;
	temp->lch = d;
	return temp;
}

void insertBucket(bucket *buck, char *s)
{
	buck->head = insertList(buck->head,s);
	buck->length++;
}


void rehash(hash *a, int h)
{
	node *temp = a->b[h]->head;
	a->b[h]->head = NULL;
	a->b[h]->length = 0;
	while(temp!=NULL)
	{
		insertBucket(a->b[hashval(temp->s,a->depth)],temp->s);
		temp = temp->next;
	}

}

hash *init()
{
	int i;
	hash *temp = (hash*)malloc(sizeof(hash));
	temp->depth = 2;
	int j = pow(2,temp->depth);
	temp->b = (bucket**)malloc(sizeof(bucket*)*j);
	
	for(i=0;i<j;i++)
	{
		temp->b[i] = createbucket(2);
	}
	return temp;
}


void search(hash *a,char *s)
{
		
	unsigned int n = hashval(s,a->depth);
	
	node *temp = a->b[n]->head;
	while(temp!=NULL)
	{
		if(!strcmp(temp->s,s))
		{
			printf("YES\n");
			return;
		}
		temp = temp->next;
	} 
	printf("NO\n");

}

bucket **reallocHT(bucket **a, int size)
{
	bucket **temp = (bucket**)malloc(sizeof(bucket*)*size);
	int i,j;
	j = size/2;
	for(i=0;i<j;i++)
	{
		temp[i] = a[i];
		temp[i+j] = a[i];
	}
	free(a);
	return temp;
}


void insert(hash *a,char *s)
{
	unsigned int h = hashval(s,a->depth);
	bucket *temp;
	
	if(a->b[h]->length < bucket_l)    // less than C case
	{
		insertBucket(a->b[h],s);
		return;
	}
	else if(a->b[h]->lch == a->depth)    // d==db case
	{
		int i = pow(2,a->depth),j,k;
		a->depth++;
		j = pow(2,a->depth);
		a->b = reallocHT(a->b,j); // dynamically increasing the depth (facing problem with realloc() function)
		
		if(a->b == NULL) {printf("error");}
		
		a->b[h]->lch = a->depth;
		temp = createbucket(a->depth); //allocating new bucket b'
	}
	else if(a->b[h]->lch < a->depth)  // d>db case
	{
		a->b[h]->lch++;
		bucket *temp = createbucket(a->b[h]->lch); // allocating new bucket b'
	}
	int i;
	int k = pow(2,a->depth);
	int j = pow(2,a->depth-1);
	
	for(i=j;i<k;i++)
	{
		if(a->b[h] == a->b[i])
		{
			a->b[i] = temp;
		}
	}
	rehash(a,h);  // rehashing needed for the elements in b , so that they possibly be splitted
	insert(a,s);  // recursive call
}

void print(hash *h)
{
	int n = pow(2,h->depth);
	int i;
	node *temp;
	
	for(i=0;i<n;i++)
	{
		temp = h->b[i]->head;
		printf("%d ",h->b[i]->lch);
		while(temp)
		{
			printf("%s ",temp->s);
			temp = temp->next;
		}
		printf("\n");
	}

}

int main()
{
    	hash *h = init();
  
	insert(h,"abc");
	insert(h,"bac");
	insert(h,"gab");
	insert(h,"kab");
	print(h);
	search(h,"kab");
	search(h,"abc");
	search(h,"ijklmnopqrst");

}

