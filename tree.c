#include<stdio.h>
#include<stdlib.h>

struct tree {
	int data;
	int no;
	struct tree *left;
	struct tree *right;
	struct tree *parent;
	};
	
	
	
	
struct tree *search(struct tree *node,int key) // function used to search a key in the tree
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
	return node;
}
	
struct tree* succ(struct tree *node,int key)
{
	node = search(node,key);
	
	if(node)
	{
		if(!node->right)    // if node->right is null we have to find the first ancestor whose left child is an ancestor of        
		{                                 // former node during moving up towards root
			while(node->parent)            
			{
				if(node->parent->left == node)
				return node->parent;
				node = node->parent;
			}
		}
		else                  //if node->right isn't null, we if have to find left most child of the subtree rooted at node->right
		{                                  //if no left child of node->right then node->right itself the successor
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
	node = search(node,key);
	
	if(node)
	{
		if(!node->left)        // if node->left is null we have to find the first ancestor whose right child is an ancestor of  
		{                                      // former node during moving up towards root
			while(node->parent)
			{
				if(node->parent->right == node)
					return node->parent;
				
				node = node->parent;
			}
		}
		else                //if node->left isn't null, we if have to find right most child of the subtree rooted at node->right
		{                                      //if no right child of node->left then node->left itself the successor
			node = node->left;
			
			while(node->right)
				node = node->right;
			
			return node;
		}
	}
			
	return NULL;
}
	
void update(struct tree *p,int i) //function is used to update the no of nodes rooted at particular node after any insertion or deletion
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
	
	if(!(*root))   // when no node in the tree
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
		struct tree *temp2;                 //temp2 is used to store the address of the parent of the newly created node
		while(temp)
		{
			temp2 = temp;
			if(temp->data>key)           //malloc hasn't been done independently because, it may so happen that user wants to
			{                            //enter duplicate data, but we already did the malloc (wastage of space)
				temp = temp->left;
			}
			else if(temp->data<key)
			{
				temp = temp->right;
			}
			else
			{
				printf("Duplicate values are not allowed\n");
				return;
			}
		}
		t = (struct tree*)malloc(sizeof(struct tree));
		t->data = key;
		t->no = 1;
		t->left = NULL;
		t->right = NULL;
		if(temp2->data>key)
		{
			temp2->left = t;
			t->parent = temp2;
		}
		if(temp2->data<key)
		{
			temp2->right = t;
			t->parent = temp2;
		}
		update(temp2,1); // update the no of nodes value(i.e no) for the proper ancestors of the newly created node
	}
}
	
void findRank(struct tree *root,int key)
{
	root = search(root,key);
	
	if(root)
	{	
		int r = 1;
		if(root->left)            
		r = r+root->left->no;  // assigning the rank considering "root" as root node. now we will move up towards root
		while(root->parent)
		{
			if(root->parent->right == root)  //the rank will increase only when one of it's ancestor is right child of another 
			{                                           //ancestor
				
				if(root->parent->left)  // at this moment if we find left subtree the rank will again increase by the no of 
					r = r+root->parent->left->no;      // nodes rooted at left subtree
					
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
	int j;
	
	if(r && i>0)
	{
		if(r->left)
		j = r->left->no;
		else
		j = 0;
		
		if(j == i-1)   //if we come to any 2 extreme we will return the data if i=1 
			return r->data;
		                    
		
		else if(j>(i-1))           //if the value of i is less than no of nodes considering left subtree and root then 
			return findIth(r->left,i);                     // search in the left subtree
		else
			return findIth(r->right,i-j-1); // otherwise go for i-(no of nodes considering left subtree and root)
	}                                                                // -th element in the right subtree
	return -1;
}

void transplant(struct tree **root,struct tree *a,struct tree *b)
{
	if(!a->parent)         // deleting the root
	{
		*root = b;
	}      
	else
	{
		
		if(a->parent->right == a)
			a->parent->right = b;
		
		else
			a->parent->left = b;
		
	}
	if(b)
	{
		b->parent = a->parent;
		b->no = a->no;
	}
}

void delete(struct tree **r, int key)
{
	struct tree *temp = *r;
	
	temp = search(temp,key);
	if(temp)
	{
		if(!temp->right) // when the node has only left child or no child
		{
			transplant(r,temp,temp->left);
			if(temp->left)
			update(temp->left,-1);
			else
			update(temp->parent,-1);
			free(temp);
		}
		else if(!temp->left)  //when the node has only right child
		{
		        transplant(r,temp,temp->right);
			update(temp->right,-1);
			free(temp);
		}
		else                               // when the node has two child
		{
			struct tree *is = succ(temp,key);       // successor is used to replace the position of the node to be deleted
			struct tree *t; 
			t = is;     // storing the pointer from which we have to update the "no" value 
			
			if(is->parent != temp)          // when the successor isn't the right child of the node
			{
				if(!is->right) //if there is no is->right no point in updating from that is position
				t = is->parent;
				transplant(r,is,is->right); // occupying position of is by is->right, because there's no left child
				is->right = temp->right;
				temp->right->parent = is;
			}
			transplant(r,temp,is);                        
			is->left = temp->left;                      
			temp->left->parent = is;
			update(t,-1);
			free(temp);
		}	
		 
	}
	else
		printf("The key does not exist in the tree\n");			
}

void destroy(struct tree *root)
{
	if(root == NULL)
	return;
	destroy(root->left);
	destroy(root->right);
	free(root);
}

void show(struct tree *r)
{
	if(r == NULL)
	return;
	show(r->left);
	printf("%d %d\n",r->data,r->no);
	show(r->right);
	
}

void split(struct tree *root, struct tree **a, struct tree **b,int key)
{
	int l;                      // keeping track if we are going through left spine or not
	struct tree *temp1,*temp2;
	while(root)
	{
		if(root->data>key)
		{
			if((*b) == NULL)
			{
				*b = root;
				l = 1;
			}
			if(l == 0)
			temp2->left = root;
			temp2 = root;
			root = root->left;
			l = 1;
		}	
		else 
		{
			if((*a) == NULL)
			{
				*a = root;
				l = 0;
			}
			if(l == 1)
			temp1->right = root;
			temp1 = root;
			root = root->right;
			l = 0;
		}
		
	}
	temp2->left = NULL;
	temp1->right = NULL;
}

int main()
{

	struct tree *root = NULL;
	struct tree *temp;
	struct tree *a = NULL;
	struct tree *b = NULL;
	int n,k;
	while(1)
	{
		printf("Enter your choice\n");
		printf("1.Insert\n2:Delete\n3:Successor\n4:Predecessor\n5:findRank\n6:findIth\n7:show\n8:split\n9:exit\n");
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
				if(k>root->no || k<=0)
				printf("out of range\n");
				else
				printf("%d\n",findIth(root,k));
				break;
			
			case 7: if(root == NULL)
				printf("tree is empty\n");
				else
				show(root);
				break;
				
			case 8: printf("enter the key to split: ");
				scanf("%d",&k);
				split(root,&a,&b,k);
				show(a);
				printf("\n");
				show(b);
				printf("\n");
				break;
			case 9: destroy(root);
				if(a!=root)
				destroy(a);
				else
				destroy(b);
				return 0;
				
		       default: printf("Wrong Choice");
		}
	}
}


	
