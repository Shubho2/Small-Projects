#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <errno.h>


int main(int argc, char *argv[])
{
	if(argv[1] == NULL)
	{
		printf("mkdir: missing operand");
		printf("Try 'mkdir help' for more information.\n");
	}
	else if(!strcmp(argv[1],"help"))
	{
		printf("Usage: mkdir <directory name>\n");
	}
    else if (mkdir(argv[1],0777) < 0)
    {
        int err = errno;
        switch(err){
        	case EROFS:
                printf("Error: That's a file name.\n");
                break;   
            case EEXIST:
                printf("Error: Directory already exists.\n");
                break;
            case EACCES:
                printf("Error: Permission denied!\n");
                break;
        }
    }
}