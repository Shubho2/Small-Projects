#include<stdio.h>
#include<stdlib.h>

struct t1 {
	int data;
	int height;
	struct t1 *left;
	struct t1 *right;
};

struct t2{
	int data;
	char col;
	struct t2 *parent;
	struct t2 *left;
	struct t2 *right;
};

int high(struct t1* root)
{
	if(root == NULL)
		return 0;
	else 
		return root->height;
}

int getBalance(struct t1* root)
{
	return (high(root->left) - high(root->right));
}

int highR(struct t2* root)
{
	if(root == NULL)
		return 0;
	else 
		return (1 + ((highR(root->left)>highR(root->right))?highR(root->left):highR(root->right)));
}

struct t1* leftRotateA(struct t1 *a)
{
	struct t1 *temp = a->right;
	a->right = temp->left;
	temp->left = a;
	a->height = 1 + ((high(a->left)>high(a->right))?high(a->left):high(a->right));
	temp->height = 1 + ((high(temp->left)>high(temp->right))?high(temp->left):high(temp->right));
	

	return temp;
}

struct t1* rightRotateA(struct t1 *a)
{
	struct t1 *temp = a->left;
	a->left = temp->right;
	temp->right = a;
	
	a->height = 1 + ((high(a->left)>high(a->right))?high(a->left):high(a->right));
	temp->height = 1 + ((high(temp->left)>high(temp->right))?high(temp->left):high(temp->right));
	return temp;
}


void leftRotateR(struct t2 **root,struct t2 *a)
{
	struct t2 *temp = a->right;
	a->right = temp->left;
	
	if(temp->left)
		temp->left->parent = a;
	
	temp->parent = a->parent;
	
	if(!a->parent)
		*root = temp;
		
	else if(a->parent->left == a)
		a->parent->left = temp;
	else
		a->parent->right = temp;
	
	
	temp->left = a;
	a->parent = temp;
	
}

void rightRotateR(struct t2 **root,struct t2 *a)
{
	struct t2 *temp = a->left;
	a->left = temp->right;
	
	if(temp->right)
		temp->right->parent = a;
	
	temp->parent = a->parent;
	
	if(!a->parent)
		*root = temp;
		
	else if(a->parent->left == a)
		a->parent->left = temp;
	else
		a->parent->right = temp;
	
	temp->right = a;
	a->parent = temp;
	
}

struct t1* createnodeA(int k)
{
	struct t1* temp = (struct t1*)malloc(sizeof(struct t1));
	temp->data = k;
	temp->left = temp->right = NULL;
	temp->height = 1;
	return temp;
}
struct t2* createnodeR(int k)
{
	struct t2* temp = (struct t2*)malloc(sizeof(struct t2));
	temp->data = k;
	temp->left = temp->right = temp->parent = NULL;
	temp->col = 'R';
	return temp;
}



struct t1* insertA(struct t1 *root, int k)
{
	if(root == NULL)
	{
         return createnodeA(k);
	}
	else if(root->data > k)
	{
		root->left = insertA(root->left,k);
	}
	else if(root->data < k)
		root->right = insertA(root->right,k);

	else                // duplicate keys not allowed
		return root;

	if(getBalance(root)>1 && k<root->left->data)  //left left case
	{
		root = rightRotateA(root);
	}
	else if(getBalance(root)>1 && k>root->left->data) //left right case
	{
		root->left = leftRotateA(root->left);
		root = rightRotateA(root);
	}
	else if(getBalance(root)<-1 && k>root->right->data) //right right case
	{
		root = leftRotateA(root);
	}
	else if(getBalance(root)<-1 && k<root->right->data) //right left case
	{
		root->right = rightRotateA(root->right);
		root = leftRotateA(root);
	}
	else
	{
		root->height = (1 + ((high(root->left)>high(root->right))?high(root->left):high(root->right)));
	}

	return root;


}
void show(struct t1 *root)
{
	if(root == NULL)
		return;
	show(root->left);
	printf("%d %d\n",root->data,root->height);
	show(root->right);
}

void showR(struct t2 *root)
{
	if(root == NULL)
		return;
	showR(root->left);
	printf("%d %c\n",root->data,root->col);
	showR(root->right);
}
void fixViolation(struct t2 	**root,struct t2 *p)
{
	struct t2 *temp;  // uncle
	while(p->parent && p->parent->col == 'R')
	{
		if(p->parent == p->parent->parent->left)
		{
			temp = p->parent->parent->right;
			
			if(temp && temp->col == 'R')
			{
				p->parent->col = 'B';
				temp->col = 'B';
				p->parent->parent->col = 'R';
				p = p->parent->parent;
			}
			else
			{ 
				if(p == p->parent->right)
				{
			 		p = p->parent;
					leftRotateR(root,p);
				}
			
				p->parent->col = 'B';
				p->parent->parent->col = 'R';
				rightRotateR(root,p->parent->parent);
			}	
		}
		else
		{
			temp = p->parent->parent->left;
			
			if(temp && temp->col == 'R')
			{
				p->parent->col = 'B';
				temp->col = 'B';
				p->parent->parent->col = 'R';
				p = p->parent->parent;
			}
			else
			{ 
				if(p == p->parent->left)
				{
			 		p = p->parent;
					rightRotateR(root,p);
				}
			
				p->parent->col = 'B';
				p->parent->parent->col = 'R';
				leftRotateR(root,p->parent->parent);
			}
		}
	}
	(*root)->col = 'B';
}


void insertR(struct t2 **root,int k)
{
	struct t2 *r = *root;
	if (r == NULL)
	{
		*root = createnodeR(k);
		(*root)->col = 'B';
		return;
	}
	struct t2 *temp;
	while(r)   // considering no duplication
	{
		if(r->data>k)
		{
			temp = r;
			r = r->left;
		}
		else
		{
			temp = r;
			r = r->right;
		}
	}
	struct t2 *p = createnodeR(k);
	
	if(temp->data>k)
		temp->left = p;
	else
		temp->right = p;
	
	p->parent = temp;
	
	if(p->col == 'R' && temp->col == 'R')
		fixViolation(root,p);

}


int main()
{
	struct t2* root = NULL;
	insertR(&root,11);
	insertR(&root,8);
	insertR(&root,14);
	insertR(&root,9);
	insertR(&root,10);
	insertR(&root,7);
	showR(root);
	printf("%d\n",highR(root));
	printf("\n");
	struct t1* root1 = NULL;
	root1 = insertA(root1,11);
	root1 = insertA(root1,8);
	root1 = insertA(root1,14);
	root1 = insertA(root1,9);
	root1 = insertA(root1,10);
	root1 = insertA(root1,7);
	show(root1);
	printf("%d\n",high(root1));
	
}
