////????? treap : BST + MINHEAP


#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>

typedef struct treap{
	int k;
	int p;
	struct treap *left;
	struct treap *right;
	}treap;	


treap* search(treap* root, int key)
{
    // Base Cases: root is null or key is present at root
    if (root == NULL || root->k == key)
       return root;
     
    // Key is greater than root's key
    if (root->k < key)
       return search(root->right, key);
  
    // Key is smaller than root's key
    return search(root->left, key);
}

treap *createnode(int key)
{
	treap *root = (treap*)malloc(sizeof(treap));
	root->k = key;
	root->p = rand()%100;
	root->left = root->right = NULL;
}



treap *leftRotate(treap *root)
{
	treap *x = root->right;
	root->right = x->left;
	x->left = root;
	return x;
}

treap *rightRotate(treap *root)
{
	treap *x = root->left;
	root->left = x->right;
	x->right = root;
	return x;
}


treap* insert(treap *root,int key)
{
	if(!root)
	return createnode(key);
	if(root->k > key)
	{
		root->left = insert(root->left,key);
		if(root->left->p<root->p)
		root = rightRotate(root);
		
	}
	else if(root->k < key)
	{
		root->right = insert(root->right,key);
		if(root->right->p<root->p)
		root = leftRotate(root);
	}
	
	return root;   // Avoiding Duplicate keys
	
}

treap* insertSpecial(treap *root,int key)
{
	if(!root)
	{
		treap *root = (treap*)malloc(sizeof(treap));
		root->k = key;
		root->p = -1;
		root->left = root->right = NULL;
		return root;
	}
	if(root->k > key)
	{
		root->left = insertSpecial(root->left,key);
		if(root->left->p<root->p)
		root = rightRotate(root);
		
	}
	else if(root->k < key)
	{
		root->right = insertSpecial(root->right,key);
		if(root->right->p<root->p)
		root = leftRotate(root);
	}
	
	return root;   // Avoiding Duplicate keys
	
}

treap *delete(treap *root,int key)
{
	if(!root)
	return root;
	if (key < root->k)
        	root->left = delete(root->left, key);
    	else if (key > root->k)
        	root->right = delete(root->right, key);
        	
        else if(root->k == key)
        {
        	if(root->left == NULL)
        	{
			treap *temp = root;
			root = root->right;
        		free(temp);
        	}
        	else if(root->right == NULL)
        	{
			treap *temp = root;
			root = root->left;
        		free(temp);
        	}
        	else if(root->left->p < root->right->p)
        	{
        		root = rightRotate(root);
        		root->right = delete(root->right,key);
        	}
        	else
        	{
        		root = leftRotate(root);
        		root->left = delete(root->left,key);
        	}
        }
        
        return root;

}

void inorder(treap* root)
{
    if (root)
    {
        inorder(root->left);
        printf("key: %d priority: %d\n",root->k,root->p);
        inorder(root->right);
    }
}

treap *join(treap *t1,treap *t2)  // all the keys in t1 must be less than all the keys in t2
{
	treap *temp = (treap*)malloc(sizeof(treap));
	temp->p = 100;
	temp->k = 10;
	temp->left = t1;
	temp->right = t2;
	temp = delete(temp,10);
	return temp;
}

treap **split(treap *t,int k)
{
	treap **a = (treap**)malloc(sizeof(treap*)*2);
	t = insertSpecial(t,k);
	a[0] = t->left;
	a[1] = t->right;
	printf("%d %d\n",t->k,t->p);
	free(t);
	return a;
}





int main()
{
	srand(time(NULL));
 	treap **a;
        treap *root = NULL;
        root = insert(root, 50);
        root = insert(root, 30);
        root = insert(root, 20);
        root = insert(root, 40);
        root = insert(root, 70);
        root = insert(root, 60);
        root = insert(root, 80);
	inorder(root);
	printf("\n");
	//root = delete(root, 20);
	//root = delete(root, 70);
	//inorder(root);
	a = split(root,45);
	inorder(a[0]);
	printf("\n");
	inorder(a[1]);
	printf("\n");
	root = join(a[0],a[1]);
	inorder(root);
	return 0;
}
