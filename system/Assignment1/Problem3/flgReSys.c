#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>


#define f 10 // No of flights
#define c 150 // Capacity of each flight
#define MAX  5 // Maximum no active queries at any given time
#define t 20 // No of threads(users) to be created

sem_t bin_sem[MAX]; // used for mutual exclusion
pthread_rwlock_t lock[f]; // used for read-write lock between threads
int flag = 1; // flag is used to terminate the threads
	
typedef struct{
	bool valid; // valid if 1
	char q_type[10]; // query type
	int q_time; // query time
	int f_no; // flight Number
	int t_no; // thread no (user)
	//int seats; // No of seats user wants to cancel or book
}query;
	
query table[MAX]; // shared table for a query window of size MAX
	
typedef struct{
	int thread_no; // Thread no
	int *shm; // shared memory for flight status
	}info;

int* init(int *shm_id)
{
	int *shmaddr,i;
	const int shm_size = f;
	/* Allocate a shared memory segment. */
  	*shm_id = shmget (IPC_PRIVATE, shm_size * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
  	
  	if(*shm_id == -1) {
  		perror("Shared memory error");
  		return 0;
  	}
  	
  	/* Attach the shared memory segment. */
 	shmaddr = (int*) shmat (*shm_id, 0, 0);
 	
 	for(i=0;i<f;i++)
 	shmaddr[i] = c; // Initializing by the capacity of the flight
 	
 	// Initialization of the table entries as invalid
 	for(i=0;i<MAX;i++)
 		table[i].valid = false; 
 	
 	return shmaddr;
}


void *query_processing(void *arg)
{
	//printf("came...\n");
	int seats,i;
	info *arg_q = (info*) arg;
	int booking[f] = {0}; // booking information for each flight
	int min_timestamp;

	while(flag)
	{
		// finding the query with minimum timestamp
		min_timestamp = INT_MAX;
		for(i=0; i<MAX; i++)
		{
			if(arg_q->thread_no == table[i].t_no && table[i].valid)
			{
				if(min_timestamp>table[i].q_time)
				min_timestamp = table[i].q_time;
			}
				
		}

		for(i=0; i<MAX; i++)
		{
			
			if((arg_q->thread_no == table[i].t_no) && table[i].valid && min_timestamp == table[i].q_time)
			{
				sem_wait(&bin_sem[i]); // Decrement count of the semaphore
				//printf("Thread id: %d %s\n",arg_q->thread_no,table[i].q_type);
				
				table[i].valid = false;
				
				if(!strcmp(table[i].q_type,"Inquiry"))
				{
					pthread_rwlock_rdlock(&lock[table[i].f_no-1]); // Read lock
		
					printf("The no of seats available in flight no %d is %d\n",table[i].f_no,arg_q->shm[table[i].f_no-1]);
			
					pthread_rwlock_unlock(&lock[table[i].f_no-1]);	// Unlock
				}
				else if(!strcmp(table[i].q_type,"Book"))
				{
					pthread_rwlock_wrlock(&lock[table[i].f_no-1]); // Write lock
		
					printf("How many seats do you want to book(Must be between 2-5)\n");
					scanf("%d",&seats);
					
					if(seats>=2 && seats<=5 && seats <= arg_q->shm[table[i].f_no-1])
					{
						arg_q->shm[table[i].f_no-1] -= seats;
						booking[table[i].f_no-1] += seats;
					}
					else
						printf("Wrong Input\n");
		
					pthread_rwlock_unlock(&lock[table[i].f_no-1]); // Unlock
				}
				else if(!strcmp(table[i].q_type,"Cancel"))
				{
					pthread_rwlock_wrlock(&lock[table[i].f_no-1]); // Write Lock
		
					printf("How many seats do you want to cancel\n");
					scanf("%d",&seats);
					
					if(booking[table[i].f_no-1] >= seats)
					{
						arg_q->shm[table[i].f_no-1] += seats;
						booking[table[i].f_no-1] -= seats;
					}
					else
						printf("Wrong input\n");
		
					pthread_rwlock_unlock(&lock[table[i].f_no-1]); // Unlock
				}
				
				sem_post(&bin_sem[i]); // Increment count of the semaphore
			}

		}
	}
	//printf("terminated..\n");
	/* terminate the thread */
   	pthread_exit(NULL);
   	
}


void spawnthreads(int *shmaddr)
{
	pthread_t *tids = (pthread_t*)malloc(sizeof(pthread_t)*t);
	info *arg[t];
	int i,q_time,f_no,t_no,ret,seats;
	char q_type[10];
	FILE *fp = fopen("b","r");
	
	// setting timer for the server
	time_t endwait,start;
	start = endwait = time(NULL); 
	time_t seconds = 300; // five minutes
	
	// Initializing binary semaphores
	for(i=0; i<MAX; i++)
		sem_init(&bin_sem[i], 0, 1);
	
	
	for(i = 0; i < f; i++)
		pthread_rwlock_init(&lock[i],NULL); // Initializing thread read-write lock
	
    for (i = 0; i < t; i++)
	{
		arg[i] = (info*)malloc(sizeof(info));
		arg[i]->thread_no = i+1;
		arg[i]->shm = shmaddr;
		
		ret = pthread_create(&tids[i], NULL, query_processing, arg[i]); // creating threads

		if(ret != 0)
		    perror("pthread create error");
	}
	
	while(seconds>(endwait-start))
	{
			
		for(i=0;i<MAX;i++)
		{
			if(table[i].valid == 0)
			{
				sem_wait(&bin_sem[i]); // Decrement count of the semaphore
					
				if(fscanf(fp,"%d %s %d %d",&q_time,q_type,&f_no,&t_no)!= EOF) // Reading user queries
				{
					table[i].valid = true;
					strcpy(table[i].q_type,q_type);
					table[i].q_time = q_time;
					table[i].f_no = f_no;
					table[i].t_no = t_no;

					// if(!strcmp(q_type,"Book"))
					// {
					// 	// printf("How many seats do you want to book(Must be between 2-5) (user no : %d)\n",t_no);
					// 	// scanf("%d",&seats);
					// 	// table[i].seats = seats;
					// }
					// else if(!strcmp(q_type,"Cancel"))
					// {
					// 	// printf("How many seats do you want to cancel (user no : %d)\n",t_no);
					// 	// scanf("%d",&seats);
					// 	// table[i].seats = seats;
					// }
				}
				else
				{
					sem_post(&bin_sem[i]);
					break;
				}
				sem_post(&bin_sem[i]); // Increment count of the semaphore
			}
		}
		endwait = time(NULL);
	}
	
	// checking if all the taken queries has been satisfied or not
	while(1)
	{
		//printf("here\n");
		for(i=0;i<MAX;i++)
		{
			sem_wait(&bin_sem[i]);
			
			if(table[i].valid)
			{
				sem_post(&bin_sem[i]);
				break;
			}
			else
				sem_post(&bin_sem[i]);
		}
		
		if(i==MAX)
		{
			flag = 0;
			break;
		}
	}
	
	for (i = 0; i < t; ++i)
    {
		ret = pthread_join(tids[i],NULL); // joining threads
		
		if(ret != 0)
		    perror("pthread join error");
		    
		else
			free(arg[i]);
	}
	
	// Closing file pointer
	fclose(fp);

	// Destroying thread read-write lock
	for(i=0; i<f; i++)
		pthread_rwlock_destroy(&lock[i]); 
		
	// Destroying semaphore
	for(i=0; i<MAX; i++)
		sem_destroy(&bin_sem[i]);
		
	
	/* Detach the shared memory segment. */
  	shmdt (shmaddr);	
	
	// Deallocating memory	
    free(tids);
}


void spawnchild(int *shmaddr)
{
	pid_t pid = fork();
	
	if(pid < 0)
	{
		printf("Error forking child");
	}
	else if(pid == 0)
	{
		spawnthreads(shmaddr);
		exit(0);
	}
	else
	wait(NULL);
	// Waiting for child to stop running
}

void show_status(int *flights)
{
	int i;
	printf("Flight Number  Available seats  Booked Seats\n");
	for(i=0; i<f; i++)
	{
		printf("%d\t\t%d\t\t%d\n",i+1,flights[i],150-flights[i]);
	}
}

int main()
{
	int shm_id;
	int *shmaddr = init(&shm_id);
	
	spawnchild(shmaddr);
	
	show_status(shmaddr);
	
 	/* Detach the shared memory segment. */
  	shmdt(shmaddr);
  	
  	/* Deallocate the shared memory segment.*/
  	shmctl(shm_id, IPC_RMID, 0);
  	
  	return 0;
}