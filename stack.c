#include<stdio.h>
#include<stdlib.h>

struct stack{
	char *a;
	int top;
	int size;
};

int isFull(struct stack *s)
{
	if(s->top == s->size-1)
	return 1;
	else return 0;
}

int isEmpty(struct stack *s)
{
	if(s->top == -1)
	return 1;
	else return 0;
}

void push(struct stack *s, char ch)
{
	if(isFull(s))
	{
			s->a = (char*)realloc(s->a,2*s->size*sizeof(char));
			s->a[++s->top] = ch;
			s->size = 2*s->size;
	}
	else
	s->a[++s->top] = ch;
}

int pop(struct stack *s)
{
	if(!isEmpty(s))
	{
		if((s->top+1) <= (s->size/4))
		{
			s->a = (char*)realloc(s->a,sizeof(char)*s->size/2);
			s->size = s->size/2;
			s->top--;
		}
		else
		s->top--;
		return 1;
	}
	else
	return -1;	
}

struct stack *createStack()
{
	struct stack *s = (struct stack*)malloc(sizeof(struct stack));
	s->a = (char*)malloc(sizeof(char));
	s->top = -1;
	s->size = 1;
	return s;
}	

int main()
{
	int i;
	struct stack *s = createStack();
	char a[100];
	printf("Enter the expression\n");
	scanf("%s",a);
	for(i=0;a[i]!='\0';i++)
	{
		if(a[i]=='(' || a[i] == '{' || a[i] == '[')
		push(s,a[i]);
		else if(s->top == -1 && (a[i] == ')' || a[i] == '}' || a[i] == ']'))
		{
			printf("no\n");
			return 0;
		}
		else if(a[i] == ')' && s->a[s->top] == '(')
		pop(s);
		else if(a[i] == '}' && s->a[s->top] == '{')
		pop(s);
		else if(a[i] == ']' && s->a[s->top] == '[')
		pop(s);
		else if(a[i] == ')' && (s->a[s->top] == '{' || s->a[s->top] == '['))
		{	
			printf("no\n");
			return 0;
		}
		else if(a[i] == '}' && (s->a[s->top] == '(' || s->a[s->top] == '['))
		{	
			printf("no\n");
			return 0;
		}
		else if(a[i] == ']' && (s->a[s->top] == '{' || s->a[s->top] == '('))
		{	
			printf("no\n");
			return 0;
		}
		
	}
	if(s->top == -1)
	printf("yes\n");
	else
	printf("no\n");
	free(s->a);
	free(s);
	return 0;
}
