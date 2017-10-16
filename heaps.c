
// designing height biased and weight biased min-heaps


#include<stdio.h>
#include<stdlib.h>

typedef struct tree1{
	int key;
	int sx;
	struct tree1 *left;
	struct tree1 *right;
	}t1;
	
typedef struct tree2{
	int key;
	int wx;
	struct tree2 *left;
	struct tree2 *right;
	}t2;


t1 *createnodeH(int k)
{
	t1 *newNode = (t1*)malloc(sizeof(t1));
	newNode->key = k;
	newNode->sx = 1;
	newNode->left = newNode->right = NULL;
	return newNode;
}

t2 *createnodeW(int k)
{
	t2 *newNode = (t2*)malloc(sizeof(t2));
	newNode->key = k;
	newNode->wx = 1;
	newNode->left = newNode->right = NULL;
	return newNode;
}

t1* meldH(t1 *a,t1 *b) // this meld should done with recursion to update the sx which can't be known beforehand
{                                  // although it doesn't matter because O(logn + logn) = O(logn)
	t1 *root;
	if(!a)
		return b;
	else if(!b)
		return a;
	
	else
	{
		if(a->key<=b->key)
		{
			root = a;
			a->right = meldH(a->right,b);
		}
		else
		{
			root = b;
			b->right = meldH(b->right,a);
		}
	}	
	if(!root->left)
	{
		root->left = root->right;
		root->right = NULL;
	}
	else
	{
		if(root->right && (root->left->sx < root->right->sx))
		{
			t1 *temp = root->left;
			root->left = root->right;
			root->right = temp;
		}
		root->sx =  (root->right) ? (1+root->right->sx) : 1;
	}
	return root;
}


t2* meldW(t2 *a, t2 *b)  // this meld can be done without recursion i.e iteratively
{                       // although it doesn't matter because O(logn + logn) = O(logn)
	t2 *root;
	if(!a)
		return b;
	else if(!b)
		return a;
	
	else
	{
		int k;
		t2 *temp;
		if(a->key<=b->key)
		{
			root = a;
			if(a->left)   
			{
				k = (root->right) ? (root->right->wx+b->wx) : b->wx;
				if(k<=root->left->wx)
				root->right = meldW(root->right,b);
				else
				{
					temp = root->right;
					root->right = root->left;
					root->left = meldW(temp,b);
				}
			}
			else           //if it doesn't have lChild then also it doesn't have rChild(property of WBLT)
			{                              // so, just hang the b as left child
				root->left = b;
			}
				
		}
		else
		{
			root = b;
			if(root->left)   
			{
				k = (root->right) ? (root->right->wx+a->wx) : a->wx;
				if(k<=root->left->wx)
				root->right = meldW(root->right,a);
				else
				{
					temp = root->right;
					root->right = root->left;
					root->left = meldW(temp,a);
				}
			}
			else           //if it doesn't have lChild then also it doesn't have rChild(property of WBLT)
			{                              // so, just hang the b as left child
				root->left = a;
			}
		}
		root->wx = (root->right) ? (1+root->left->wx+root->right->wx) : (1+root->left->wx);
            //this else will execute atleast for two nodes one will be root another will be left child
            			// so, left child will always be non-null
	}	
	return root;
	
}


t1*  insertH(t1 *root,int key)
{
	t1 *temp = createnodeH(key);
	return meldH(root,temp);
}

t2*  insertW(t2 *root,int key)
{
	t2 *temp = createnodeW(key);
	return meldW(root,temp);
}

t1* deleteMinH(t1 *root)
{
	if(root)
	{
		printf("%d\n",root->key);
		t1 *a = root->left;
		t1 *b = root->right;
		free(root);
		return meldH(a,b);
	}
	printf("No element in heap");
	return NULL;
}

t2* deleteMinW(t2 *root)
{
	if(root)
	{
		printf("%d\n",root->key);
		t2 *a = root->left;
		t2 *b = root->right;
		free(root);
		return meldW(a,b);
	}
	printf("No element in heap");
	return NULL;
}

int extractMinH(t1 *root)
{
	return root->key;
}

int extractMinW(t2 *root)
{
	return root->key;
}

t1* buildH(int *a, int n)
{	
	int i;
	t1 *root = NULL;
	for(i=0;i<n;i++)
	{
		root = insertH(root,a[i]);
	}
	return root;
}


t2* buildW(int *a, int n)
{	
	int i;
	t2 *root = NULL;
	for(i=0;i<n;i++)
	{
		root = insertW(root,a[i]);
	}
	return root;
}

t1 *buildHnew(int *a, int i, int j)
{
	if(i<j)
	{
		int mid = (i+j)/2;
		t1 *root1 = buildHnew(a,i,mid);
		t1 *root2 = buildHnew(a,mid+1,j);
		return meldH(root1,root2);
	}
	else
	{
		return createnodeH(a[i]);
	}

}

t2 *buildWnew(int *a, int i, int j)
{
	if(i<j)
	{
		int mid = (i+j)/2;
		t2 *root1 = buildWnew(a,i,mid);
		t2 *root2 = buildWnew(a,mid+1,j);
		return meldW(root1,root2);
	}
	else
	{
		return createnodeW(a[i]);
	}

}

void showH(t1 *root)
{
	if(!root)
		return;
	showH(root->left);
	printf("%d %d\n",root->key,root->sx);
	showH(root->right);

}

void showW(t2 *root)
{
	if(!root)
		return;
	showW(root->left);
	printf("%d %d\n",root->key,root->wx);
	showW(root->right);

}


void destroyH(t1 *root)
{
	if(!root)
		return;
	destroyH(root->left);
	destroyH(root->right);
	free(root);
}

void destroyW(t2 *root)
{
	if(!root)
		return;
	destroyW(root->left);
	destroyW(root->right);
	free(root);
}

int main()
{
	int a[] = {7,8,5,12,13,6,20,10};
	t1 *root1 = buildHnew(a,0,7);	
	t2 *root2 = buildWnew(a,0,7);

	showH(root1);
	printf("\n");
	showW(root2);
	
	destroyH(root1);
	destroyW(root2);


}
