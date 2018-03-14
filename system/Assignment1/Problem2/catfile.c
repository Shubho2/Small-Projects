#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 

int main(int argc, char *argv[])
{
	FILE *fp[argc];
    char line[1024];
    int i;

	if (argc>10)
	{
		printf("Argument count exceeding 10\n");
		return 0;
	}
         
    if (argc == 1){
        while(fgets(line, 1024, stdin))
        {
            printf("%s",line);
        }
        return 0;
    }

    if(!strcmp(argv[1],"help"))
    {
    	printf("Usage: cat [options] [file names] - [file names] [options] <file name>\n");
    	printf("Options: -/>/>>\n");
    	exit(0);
    }

    for(i=1;i<argc;i++)
    {
    	if(!strcmp(argv[i],"-"))
    	{
    		while(fgets(line, 1024, stdin))
            {
                printf("%s",line);
            }
    	}
	    else if((fp[i] = fopen(argv[i],"r")) == NULL){
	        printf("%s: File doesn't exists!!\n",argv[i]);
	        continue;
	    }
	    while(fgets(line, 1024, fp[i])){
	        printf("%s", line);
	    }
	    fclose(fp[i]);
        printf("\n");
	}
    close(1);
}