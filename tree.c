#include<stdio.h>
#include<stdlib.h>

struct tree {
	int data;
	int no;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
	};
	
	
struct tree* succ(struct tree *node,int key)
{
	while(node)
		{
			if(node->data>key)
			{
				node = node->left;
			}
			else if(node->data<key)
			{
				node = node->right;
			}
			else
				break; 
		}
	if(node)
	{
		if(!node->right)
		{
			while(node->parent)
			{
				if(node->parent->left == node)
				return node->parent;
				node = node->parent;
			}
		}
		else
		{
			node = node->right;
			while(node->left)
				node = node->left;
			return node;
		}
	}
			
	return NULL;
}



struct tree* pred(struct tree *node, int key)
{
	while(node)
		{
			if(node->data>key)
			{
				node = node->left;
			}
			else if(node->data<key)
			{
				node = node->right;
			}
			else
				break; 
		}
	if(node)
	{
		if(!node->left)
		{
			while(node->parent)
			{
				if(node->parent->right == node)
					return node->parent;
				
				node = node->parent;
			}
		}
		else
		{
			node = node->left;
			
			while(node->right)
				node = node->right;
			
			return node;
		}
	}
			
	return NULL;
}
	
void update(struct tree *p,int i)
{
	while(p)
	{
		p->no+=i;
		p = p->parent;
	}

}
void insert(struct tree **root, int key)
{
	struct tree *t;
	int l;
	if(!(*root))
	{
		t = (struct tree*)malloc(sizeof(struct tree));
		t->data = key;
		t->no = 1;
		t->left = NULL;
		t->right = NULL;
		*root = t;
		t->parent = NULL;
	}
	else
	{
		struct tree *temp = *root;
		struct tree *temp2;
		while(temp)
		{
			if(temp->data>key)
			{
				temp2 = temp;
				temp = temp->left;
				l = 1;
			}
			else if(temp->data<key)
			{
				temp2 = temp;
				temp = temp->right;
				l = 0;
			}
			else
			{
				printf("Duplicate values not allowed\n");
				return;
			}
		}
		if(l==1)
		{
			t = (struct tree*)malloc(sizeof(struct tree));
			t->data = key;
			t->no = 1;
			t->left = NULL;
			t->right = NULL;
			temp2->left = t;
			t->parent = temp2;
		}
		if(l==0)
		{
			t = (struct tree*)malloc(sizeof(struct tree));
			t->data = key;
			t->no = 1;
			t->left = NULL;
			t->right = NULL;
			temp2->right = t;
			t->parent = temp2;
		}
		update(temp2,1);
	}
}
	
void findRank(struct tree *root,int key)
{
	while(root)
		{
			if(root->data>key)
			{
				root = root->left;
			}
			else if(root->data<key)
			{
				root = root->right;
			}
			else
				break; 
		}
	if(root)
	{	
		int r = 1;
		if(root->left)
		r = r+root->left->no;
		while(root->parent)
		{
			if(root->parent->right == root)
			{
				if(root->parent->left)
					r = r+root->parent->left->no;
					
				r+=1;
			}
			root = root->parent;
		}
		printf("%d\n",r);
	}
	else
	printf("%d doesn't exist in the tree\n",key);
}

int findIth(struct tree *r, int i)
{
	if(r && i>0)
	{
		if((r->left->no)>(i-1))
			return findIth(r->left,i);
		else if(r->left->no == (i-1))
			return r->data;
		else
			return findIth(r->right,i-(r->left->no)-1);
	}
	return -1;
}

void delete(struct tree **r, int key)
{
	struct tree *temp = *r;
	while(temp)
		{
			if(temp->data>key)
			{
				temp = temp->left;
			}
			else if(temp->data<key)
			{
				temp = temp->right;
			}
			else
				break; 
		}
	if(temp)
	{
		if(temp->left == NULL && temp->right == NULL)
		{
			if(temp->parent)
			{
				if(temp->parent->right == temp)
					temp->parent->right = NULL;
				else if(temp->parent->left == temp)
					temp->parent->left = NULL;
				update(temp->parent,-1);
			}
			free(temp);
		}
		else if(temp->left && !temp->right)
		{
			if(temp->parent->right == temp)
			{
				temp->parent->right = temp->left;
				temp->left->parent = temp->parent;
			}
			else
			{
				temp->parent->left = temp->left;
				temp->left->parent = temp->parent;
			}
			
			update(temp->parent,-1);
			free(temp);
		}
		else if(!temp->left && temp->right)
		{
			if(temp->parent->right == temp)
			{
				temp->parent->right = temp->right;
				temp->right->parent = temp->parent;
			}
			else
			{
				temp->parent->left = temp->right;
				temp->right->parent = temp->parent;
			}
			
			update(temp->parent,-1);
			free(temp);
		}
		else
		{
			struct tree *is = succ(temp,key);
			if(temp->right == is)
			{
				if(temp->parent)
				{
					temp->parent->right = temp->right;
					temp->right->parent = temp->parent;
				}
				temp->left->parent = temp->right;
				temp->right->left = temp->left;
				
				is->no = temp->no - 1;
				if(!(temp->parent))
				*r = is;
				update(is->parent,-1);
				free(temp);
			}
			else
			{
				if(temp->parent)
				{
					if(temp->parent->right == temp)	
						temp->parent->right = is;
					else
						temp->parent->left = is;
					if(is->right)
						is->right->parent = is->parent;
					is->parent->left = is->right;
					struct tree *temp2 = is->parent;
					
					
					if(temp->left)
						temp->left->parent = is;
						
					is->left = temp->left;
					
					if(temp->right)
						temp->right->parent = is;
						
					is->right = temp->right;
					
					is->parent = temp->parent;
					is->no = temp->no;
					
					update(temp2,-1);
				}
				else
				{
					if(is->right)
						is->right->parent = is->parent;
					is->parent->left = is->right;
					struct tree *temp2 = is->parent;
					
					
					
					if(temp->left)
						temp->left->parent = is;
						
					is->left = temp->left;
					
					if(temp->right)
						temp->right->parent = is;
					
					is->right = temp->right;
					
					is->no = temp->no;
					is->parent = temp->parent;
					
					update(temp2,-1);
					
					*r = is;
				}
				free(temp);
			}	
		}  
	}
	else
		printf("The key doesn not exist in the tree\n");			
}




int main()
{

	struct tree *root = NULL;
	struct tree *temp = NULL;
	int n,k;
	while(1)
	{
		printf("Enter your choice\n");
		printf("1.Insert\n2:Delete\n3:Successor\n4:Predecessor\n5:findRank\n6:findIth\n7:exit\n");
		scanf("%d",&n);
		switch(n)
		{
			case 1: printf("Enter the element: ");
			 	scanf("%d",&k);
			 	insert(&root,k);
			 	break;
			 	
			case 2: printf("Enter the element to delete: ");
				scanf("%d",&k);
				delete(&root,k);
				break;
				
			case 3: printf("Enter the element: ");
				scanf("%d",&k);
				temp = succ(root,k);
				if(temp)
					printf("%d\n",temp->data);
				else
					printf("No Successor Exist or You gave a non-existent element\n");
				break;
				
			case 4: printf("enter the element: ");
				scanf("%d",&k);
				temp = pred(root,k);
				if(temp)
					printf("%d\n",temp->data);
				else
					printf("No predecessor Exist or You gave a non-existent element\n");	
				break;	
				
			case 5: printf("enter the element: ");
				scanf("%d",&k);
				findRank(root,k);
				break;
			
			case 6: printf("enter the rank: ");
				scanf("%d",&k);
				if(k>(root->left->no+root->right->no+1))
				printf("out of range\n");
				else
				printf("%d\n",findIth(root,k));
				break;
			
			case 7: free(root);
				return 0;
		}
	}
}


	
