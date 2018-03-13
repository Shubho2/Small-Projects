#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<time.h>
#define max_level 6

typedef struct snode{
	int key;
	struct snode **next;
	} snode;

typedef struct skiplist{
	int level;
	snode *head;
	}slist;
	
slist *init()
{
	int i;
	slist *s = (slist*)malloc(sizeof(slist));
	s->head = (snode*)malloc(sizeof(snode));
	s->head->key = INT_MIN;
	s->head->next = (snode**)malloc(sizeof(snode*)*max_level);
        for (i = 0; i < max_level; i++) 
        {
		s->head->next[i] = NULL;
	} 
	s->level = 1;
	return s;
}


int rand_level() {
	int level = 1; // base level is considered 
        while(rand() < RAND_MAX / 2 && level < max_level)
		level++;
        
        return level;

}

snode *createnode(int key,int lvl)
{
	int i;
	snode *n = (snode*)malloc(sizeof(snode));
	n->next = (snode**)malloc(sizeof(snode*)*lvl);
	n->key = key;
	for(i=0;i<lvl;i++)
	{
		n->next[i] = NULL;	
	}
	return n;
}

void insert(slist *s, int key)
{
	int i;
	snode *cur = s->head;
	snode *update[max_level];
	for(i=s->level-1;i>=0;i--)
	{
		while(cur->next[i] && cur->next[i]->key < key)
			cur = cur->next[i];
			
		update[i] = cur;
	}
	cur = cur->next[0];
	if(cur == NULL || cur->key != key)     // Avoiding duplication of keys
	{
		int rlevel = rand_level();
		
		if(rlevel > s->level)
		{
			for (int i=s->level;i<rlevel;i++)
                		update[i] = s->head;	
		
			s->level = rlevel;   // updating the current level
		}
		
		snode *n = createnode(key,rlevel);
		for(i=0;i<rlevel;i++)
		{
			n->next[i] = update[i]->next[i];
			update[i]->next[i] = n;
		}
		printf("successfully inserted key\n");
	}
}

void search(slist *root, int key)
{
	snode *cur = root->head;
	int i;
	for(i=0;i<root->level;i++)
	{
		while(cur->next[i] && cur->next[i]->key < key)
			cur = cur->next[i];
	}
	if(cur->next[0] && cur->next[0]->key == key)
		printf("Key Found\n");
	else
	printf("Key not Found\n");
}

void delete(slist *root,int key)
{
	int i;
	snode *cur = root->head;
	snode *update[max_level];
	for(i=root->level-1;i>=0;i--)
	{
		while(cur->next[i] && cur->next[i]->key < key)
			cur = cur->next[i];
			
		update[i] = cur;
	}	
	cur = cur->next[0];
	if(cur->key == key)
	{
		for(i=0;i<root->level;i++)
		{
			if(update[i]->next[i] != cur)
			break;
			update[i]->next[i] = cur->next[i];		
		}
	
		while(root->level>1 && root->head->next[root->level-1] == NULL)
			root->level--;
		free(cur->next);
		free(cur);
		printf("Deleted\n");
	}
	else
		printf("Key not found\n");
}

void displayList(slist *s)
{
    printf("\n*****Skip List*****\n");
    for (int i=0;i<s->level;i++)
    {
        snode *node = s->head->next[i];
        printf("Level:%d ",i);
        while (node != NULL)
        {
            printf("%d ",node->key);
            node = node->next[i];
        }
        printf("\n");
    }
}


int main()
{
	srand((unsigned)time(0));
	slist *root = init();
	insert(root,5);
	insert(root,10);
	insert(root,50);
	insert(root,40);
	displayList(root);
	insert(root,45);
	displayList(root);
	search(root,10);
	search(root,20);
	delete(root,41);
	displayList(root);
	delete(root,10);
	displayList(root);
	return 0;

}
