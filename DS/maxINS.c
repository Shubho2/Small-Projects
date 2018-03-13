#include<stdio.h>

int findMax(int *a, int n)
{
	int l,i,max;
	int b[n];
	if(n == 1)
		return a[0];
	else if(n == 2)
		return (a[0]>a[1])?a[0]:a[1];
	else
	{
		b[0] = a[0];
		b[1] = (a[0]>a[1])?a[0]:a[1];
		for(i=2;i<n;i++)
		{
			if(b[i-1] > a[i]+b[i-2])
			b[i] = b[i-1];
			else
			b[i] = a[i] + b[i-2];		
		}	
	}
	return b[n-1];
}


int main()
{
	int t,n,i;
	scanf("%d",&t);
	while(t--)
	{
		scanf("%d",&n);
		int a[n];
		for(i=0;i<n;i++)
		scanf("%d",(a+i));
		printf("%d\n",findMax(a,n));
	}

}
