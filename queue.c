#include<stdio.h>
#include<stdlib.h>

struct qNode{
	int data;
	struct qNode *next;
	struct qNode *prev;
	};
	
struct queue{
	struct qNode *front,*rear;
	};
	
struct queue* createQueue()
{
	struct queue *q = (struct queue*)malloc(sizeof(struct queue));
	q->front = q->rear = NULL;
	return q;
}
struct qNode* create(int k)
{
	struct qNode* node = (struct qNode*)malloc(sizeof(struct qNode));
	node->data = k;
	node->prev = node->next = NULL;
	return node;
}

void enqueue(struct queue* q, int k)
{
	struct qNode *temp = create(k);
	if(q->rear == NULL)
	{
		q->front = q->rear = temp;
	}
	else
	{
		q->rear->next = temp;
		temp->prev = q->rear;
		q->rear = temp;
	}
}

struct qNode* deQueue(struct queue *q)
{
	if(q->front == NULL)
	return NULL;
	struct qNode* temp  = q->front;
	q->front = q->front->next;
	if(q->front)
	q->front->prev = NULL;
	return temp;
}

 
void show(struct queue* q)
{
	struct qNode *temp = q->front;
	while(temp!=NULL)
	{
		printf("%d ",temp->data);
		temp = temp->next;
	}
	printf("\n");
	temp = q->rear;
	while(temp!=NULL)
	{
		printf("%d ",temp->data);
		temp = temp->prev;
	}
	printf("\n");
}
	
int main()
{
	
	struct queue *q = createQueue();
	struct qNode *n;
	enqueue(q,5);
	enqueue(q,6);
	enqueue(q,9);
	n = deQueue(q);
	show(q);
	printf("%d\n",n->data);
	free(n);
	free(q);
	
}
	
