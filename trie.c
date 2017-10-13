#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct trie{
	struct trie *next[26];
	int count; // help in deletion to remove a node if there is no letter of word associate with it
	bool isEnd; // help to detect a word	
	}trie;



trie *createnode()
{
	int i;
	trie *node = (trie*)malloc(sizeof(trie));
	for(i=0;i<26;i++)
	node->next[i] = NULL;
	node->isEnd = false;
	node->count = 0;
	return node;
}

void insert(trie *root, char *ch)
{
	int l = strlen(ch);
	int i,j;
	for(i=0; i<l; i++)
	{
		j = (int)ch[i] - (int)'a';  // (int) 'a' = 97
		if(!root->next[j])
			root->next[j] = createnode();
		
		root->count++;
		root = root->next[j];	
	}
	root->isEnd = true;
}


bool search(trie *root, char *ch)
{
	int l = strlen(ch);
	int i,j;	
	for(i=0;i<l;i++)
	{
		j = (int)ch[i] - (int)'a';
		if(!root->next[j])
			return false;
		root = root->next[j];
	}
	if(root && root->isEnd)
		return true;
}


trie* deleteHelper(trie *root, char *ch)
{
	if(!ch[0])
	return root;
	int i = (int)ch[0] - (int)'a';	
	root->next[i] = deleteHelper(root->next[i],ch+1);
	root->count--;
	if(root->count == 0)
	{
		free(root);
		return NULL;
	}
	else
	return root;
}

trie *delete(trie *root,char *ch)
{
	trie *temp = root;
	int l = strlen(ch);
	int i,j;
	for(i=0;i<l;i++)
	{
		j = (int)ch[i] - (int)'a';
		if(!temp->next[j])
			return root;   // if doesn't exist don't care 
		temp = temp->next[j];
	}
	temp->isEnd = false;
	return deleteHelper(root,ch);
}

int main()
{
	trie *root = createnode();
	insert(root,"c");	
	insert(root,"abde");
	insert(root,"cdf");
	printf("%d\n",search(root,"cdf"));
	root = delete(root,"c");
	printf("%d\n",search(root,"c"));
	return 0;
}
