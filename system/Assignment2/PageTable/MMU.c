#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <time.h>


typedef struct {
	bool valid;
	int frame;
	bool dirty;
	pid_t requested;
	bool latest_access_bit;
}PTable;


void print(PTable *pt, int size)
{
	int i;
	printf("\n****************Page Table*****************\n");
	for(i=0;i<size;i++)
	{
		if(pt[i].valid)
		{
			printf("****************************************\n");
			printf("|%d:Valid=%d Frame=%d Dirty=%d Requested=%d|\n",i,pt[i].valid,pt[i].frame,pt[i].dirty,pt[i].requested);
			printf("****************************************\n");
		}
		else
		{
			printf("%d:Valid=%d Frame=%d Dirty=%d Requested=%d\n",i,pt[i].valid,pt[i].frame,pt[i].dirty,pt[i].requested);
		}
	}
}


int main(int argc,char *argv[])
{

	if(argc < 6)
	{
		printf("Less Argument\n");
		printf("Need 5 arguments after a.out\n");
		exit(1);
	}

	int i,pageno,page_hit=0,page_fault=0;
	FILE *fp;
	char mode;
	int ptable_size = atoi(argv[1]);
	int page_bits = atoi(argv[3]);
	int offset_bits = atoi(argv[4]);
	pid_t os_pid = atoi(argv[5]);
	key_t shm_key = ftok(".",'y');


	// calculating the size of the address string
	int address_size = (page_bits + offset_bits)/4;

	char address[address_size+3];

	printf("PID:%d\n",getpid());
	/* Allocate a shared memory segment. */
	int shm_id = shmget(shm_key, ptable_size*sizeof(PTable), 0666);

	if(shm_id == -1) {
  		perror("Shared memory error");
  		return 0;
  	}

  	/* Attach the shared memory segment. */
 	PTable *pt = (PTable*)shmat(shm_id, NULL, 0);

 	printf("MMU is attached with the pagetable\n");

 	// Reading memory trace file
 	if((fp = fopen(argv[2],"r")) == NULL)
 	{
 		perror("Error opening file\n");
 	}
	else
 	{

		while(fscanf(fp,"%s %c",address,&mode)!=EOF)
		{
			pageno = strtol(address,NULL,16)/pow(2,offset_bits);
			if(pageno > (pow(2,page_bits)-1) && pageno < 0)
			{
				printf("page number doesn't exist\n");
			}
			else
			{
				printf("\nRequest for page = %d in mode %c\n",pageno,mode);

				if(pt[pageno].valid)
				{
					page_hit++;
				}
				else
				{

					printf("\nNot present in the page table\n\n");
					
					page_fault++;
					pt[pageno].requested = getpid();
					//print(pt,ptable_size);
					kill(os_pid,SIGUSR1);
					kill(getpid(),SIGSTOP);

				}

				// After accessing the page in the memory
				for(i=0;i<ptable_size;i++)
				{
					if(i == pageno)
						pt[i].latest_access_bit = true;
					else
						pt[i].latest_access_bit = false;
				}

				if(mode == 'w' || mode == 'W')
				{
					pt[pageno].dirty = true;
				}
			}
			print(pt,ptable_size);
		}
	}

	printf("\nNo of page hit:%d and No of page fault:%d\n",page_hit,page_fault);
	/* Detach the shared memory segment. */
  	shmdt(pt);

  	// Sending signal to the os for the last time
  	kill(os_pid,SIGUSR2);
  	printf("Signal is sent to OS..\n");
 
 	return 0;
}