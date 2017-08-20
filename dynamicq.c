//Author: R. Inkulu
//Dynamic-sized circular queue
//Homework: fix bugs in the code

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
   int capacity;
   int head; //ptr[head] is the element to be dequeued
   int tail; //enqueued element is placed at ptr[tail]
   void **ptr; //ptr is a pointer to an array of void *s
} Queue;

//(q is assumed to be non-NULL in all the functions)

void initializeQueue(Queue *q, int initialCapacity) {
   q->capacity = initialCapacity; 
   q->head = -1; 
   q->tail = 0;
   q->ptr = (void**) malloc(q->capacity*sizeof(void*));
}

void destroyQueue(Queue *q) {
   free(q->ptr);
   q->ptr = NULL;
   q->head = q->tail = -1; 
}

void enqueue(Queue *q, void *data) {
   q->ptr[q->tail] = data;
   if (q->head == -1)
      q->head = 0;

   q->tail = (q->tail + 1)%q->capacity;

   if (q->tail == q->head) {
      //indicates that no place to save the enqueue
      void **tmp = q->ptr;
      int oldcapacity = q->capacity;

      //expand the buffer pointed by ptr
      q->capacity *= 2;
      q->ptr = malloc(q->capacity*sizeof(void*));
      q->tail = oldcapacity;
      int i=0;
      while(oldcapacity-- != 0) {
        q->ptr[i++] = q->ptr[q->head];
        q->head = (q->head + 1) % q->capacity;
      }
      q->head = 0;

      free(tmp);
   }

   return;
}

void *dequeue(Queue *q) {
   if (q->head == -1)
      return NULL;   //queue underflow

   void *ret = q->ptr[q->head];
   q->head= (q->head + 1)%q->capacity;
   if (q->head == q->tail) {
      //indicates queue is empty
      q->head = -1;
      q->tail = 0;
   }

   //fill the code to shrink the size of the array 
   //whenever it is required ?

   return ret;
}

int main(void) {
   Queue qObj;
   initializeQueue(&qObj, 10);

   enqueue(&qObj, "ABC");
   enqueue(&qObj, "DEFG");
   enqueue(&qObj, "HIJ");
   void *p = dequeue(&qObj);
   printf("%s\n", (const char*)p); 
   enqueue(&qObj, "LKJ");

   destroyQueue(&qObj);
}


