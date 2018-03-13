#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>


typedef struct {
	bool valid;
	int frame;
	bool dirty;
	pid_t requested;
	bool latest_access_bit;
}PTable;

typedef struct {
	int pageno;
}FTable;

int count = 0;

volatile sig_atomic_t myFlag1 = 0;
volatile sig_atomic_t myFlag2 = 1;
int disk_access = 0;

void print(FTable *ft, int size)
{
	int i;
	printf("\n************Frame Table************\n");
	for(i=0;i<size;i++)
	{
		printf("%d:Page No=%d\n",i,ft[i].pageno);
	}

	printf("************************************\n");
}


void myHandler1()
{
	myFlag1 = 1;
}

void myHandler2()
{
	myFlag2 = 0;
}

void FIFO(PTable *pt, FTable *ft, int p, int f)
{
	int i,index;

	index = count;
	count = (count+1)%f;

	if(pt[ft[index].pageno].dirty)
	{
		sleep(2); // For write back to Disk
		pt[ft[index].pageno].dirty = false;
		disk_access++;
	}

	pt[ft[index].pageno].valid = false;
	pt[ft[index].pageno].frame = -1;

	sleep(2); // For bringing the frame from Disk and Overwrite
	disk_access++;

	//updating page table
	pt[p].valid = true;
	pt[p].frame = index;

	//updating frame table
	ft[index].pageno = p;

	printf("Allocate page = %d to free frame %d\n",p,index);

}

void MRU(PTable *pt, FTable *ft, int p, int ptable_size)
{
	int i,index;
	for(i=0;i<ptable_size;i++)
	{
		if(pt[i].latest_access_bit)
		{
			index = i;
			break;
		}
	}

	if(pt[index].dirty)
	{
		sleep(2); // For write back to Disk
		pt[index].dirty = false;
		disk_access++;
	}

	pt[index].valid = false;
	
	sleep(2); // For bringing the frame from Disk and Overwrite
	disk_access++;


	// updating page table
	pt[p].frame = pt[index].frame;
	pt[index].frame = -1;
	pt[p].valid = true;

	// updating frame table
	ft[pt[p].frame].pageno = p;

	printf("Allocate page = %d to free frame %d\n",p,pt[p].frame);
}


int main(int argc,char *argv[])
{

	if(argc < 3)
	{
		printf("Less Argument\n");
		printf("Need 2 arguments after a.out\n");
		exit(1);
	}

	// Initialization
	int i,select,p,f,MMU_pid;
	int ptable_size = atoi(argv[1]);
	int no_of_frames = atoi(argv[2]);
	key_t shm_key = ftok(".",'y');
	FTable ft[no_of_frames];

	/* Allocate a shared memory segment. */
	int shm_id = shmget(shm_key, ptable_size*sizeof(PTable), IPC_CREAT | 0666);

	if(shm_id == -1) {
  		perror("Shared memory error\n");
  		return 0;
  	}

  	/* Attach the shared memory segment. */
 	PTable *pt = (PTable*)shmat(shm_id, NULL, 0);
	printf("Page Table Created\n");

 	for(i=0;i<ptable_size;i++)
 	{
 		pt[i].valid = false;
 		pt[i].frame = -1;
 		pt[i].dirty = false;
 		pt[i].requested = 0;
 		pt[i].latest_access_bit = false;
 	}
 	
 	printf("Page Table Initialized\n");

 	printf("Which Replacement policy do you want? (0 for FIFO/ 1 for MRU)\n");
 	scanf("%d",&select);

 	printf("My Process Id is %d\n",getpid());

 	// Initializing frames
 	for(i=0;i<no_of_frames;i++)
 	{
 		ft[i].pageno = -1;
 	}

 	if(signal(SIGUSR1,myHandler1) == SIG_ERR)
 	{
 		printf("Signal Error for Handling Page Fault\n");
 		exit(1);
 	}

 	if(signal(SIGUSR2,myHandler2) == SIG_ERR)
 	{
 		printf("Signal Error for Terminating the MMU\n");
 		exit(2);
 	}


 	while(myFlag2)
 	{
 		if(myFlag1)
 		{
 			for(p=0;p<ptable_size;p++)
 			{
 				if(pt[p].requested)
 					break;
 			}

 			printf("Process %d has requested for page %d\n",pt[p].requested,p);

 			for(f=0;f<no_of_frames;f++)
 			{
 				if(ft[f].pageno == -1)
 					break;
 			}

 			if(f<no_of_frames)
 			{
 				pt[p].frame = f;
 				pt[p].valid = true;
 				ft[f].pageno = p;
 				sleep(2); // for bringing the frame from disk
 				disk_access++;
 				printf("Allocate page = %d to free frame %d\n",p,f);
 			}
 			else
 			{
 				switch (select)
 				{
	 				case 0: FIFO(pt,ft,p,no_of_frames);
	 						break;
	 				
	 				case 1: MRU(pt,ft,p,ptable_size);
	 						break;
 				}
  			}

 			print(ft,no_of_frames);

 			MMU_pid = pt[p].requested;
 			pt[p].requested = 0;

 			myFlag1 = 0; // Reseting the flag
 			printf("Resume MMU operation\n");

 			kill(MMU_pid,SIGCONT);
 		}
 	}

 	printf("\nNo of Disk Access:%d\n",disk_access);

 	/* Detach the shared memory segment. */
  	shmdt(pt);
  	
  	/* Deallocate the shared memory segment.*/
  	shmctl(shm_id, IPC_RMID, 0);
  	
  	return 0;
}