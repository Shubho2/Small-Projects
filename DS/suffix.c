#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct suffix
{
    int index; // To store original index
    int rank[2]; // To store ranks and next rank pair
}suffix;
	
	
int cmp(suffix a, suffix b)
{
	return strcmp(a.suff, b.suff) < 0? 1 : 0;
}

int *buildSuff(char *a, int n)
{
	suffix suffixes[n];
	int i;
	int *suf = (int*)malloc(sizeof(int)*n);
	for(i=0; i<n; i++)
	{
		suffixes[i].index = i;
		suffixes[i].suff = (a+i);
	}
	
	qsort(suffixes,n,sizeof(suffix[0]),cmp);
	for(i=0;i<n;i++)
	{
		printf("%s %d\n",suffixes[i].suff,suffixes[i].index);
	}
	for(i=0;i<n;i++)
		suf[i] = suffixes[i].index;
	
	return suf;
}
int main()
{
	char *a = "banana";
	int i;
	int *suf = buildSuff(a,6);
	for(i=0;i<6;i++)
	{
		printf("%s %d\n",(a+suf[i]),suf[i]);
	}
}
