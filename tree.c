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
			if(temp->data>key)           //malloc hasn't been done independently because, it may so happen that user wants to
			{                            //enter duplicate data, but we already did the malloc (wastage of space)
				temp2 = temp;
				temp = temp->left;
			}
			else if(temp->data<key)
			{
				temp2 = temp;
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
	if(r && i>0)
	{
		if(!(r->left) && i==1)   //if we come to any 2 extreme we will return the data if i=1 
			return r->data;
		
		else if(r->left->no == (i-1)) // value at the root
			return r->data;                    
		
		else if((r->left->no)>(i-1))           //if the value of i is less than no of nodes considering left subtree and root then 
			return findIth(r->left,i);                     // search in the left subtree
		else
			return findIth(r->right,i-(r->left->no)-1); // otherwise go for i-(no of nodes considering left subtree and root)
	}                                                                // -th element in the right subtree
	return -1;
}

void delete(struct tree **r, int key)
{
	struct tree *temp = *r;
	
	temp = search(temp,key);
	
	if(temp)
	{
		if(temp->left == NULL && temp->right == NULL) // if the node to be deleted is a node with no child
		{
			if(temp->parent)        // deleting a leaf node
			{
				if(temp->parent->right == temp)
					temp->parent->right = NULL;
				else if(temp->parent->left == temp)
					temp->parent->left = NULL;
				update(temp->parent,-1);
			}
			else
				*r = NULL;  // deleting a root node
				
			free(temp);
		}
		else if(temp->left && !temp->right) // when the node has only left child
		{
			if(!temp->parent)         // deleting the root
				*r = temp->left;      
			else
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
			}
			free(temp);
		}
		else if(!temp->left && temp->right)  //when the node has only right child
		{
			if(!temp->parent)
				*r = temp->right;  //deleting the root
			else
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
			}
			free(temp);
		}
		else                               // when the node has two child
		{
			struct tree *is = succ(temp,key);  // successor is used to replace the position of the node to be deleted
			if(temp->right == is)              // when the successor is the right child of the node
			{
				if(temp->parent)
				{
					if(temp->parent->left == temp)       // attaching the descendants to the parent of the node to 
					temp->parent->left = temp->right;    // be deleted
					
					else
					temp->parent->right = temp->right;
					
					temp->right->parent = temp->parent;
				}
				temp->left->parent = temp->right;
				temp->right->left = temp->left;
				
				is->no = temp->no - 1;
				if(!(temp->parent))   // if the node (to be deleted) is the root
				{
					*r = is;
					is->parent = NULL;
				}
				update(is->parent,-1);
				free(temp);
			}
			else       // when the successor isn't the right child
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
				if(k>root->no || k<=0)
				printf("out of range\n");
				else
				printf("%d\n",findIth(root,k));
				break;
			
			case 7: free(root);
				return 0;
		}
	}
}


	
