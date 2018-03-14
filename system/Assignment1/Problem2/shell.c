#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

#define MAXCOM 1000 // Maximum number of letters to be supported
#define MAXARGS 20  // Maximum no of arguments allowed is 19
char buf[100];

// Function to take input
int takeInput(char* str)
{
    char* buf;
    char cwd[1024];
    char str1[1024];
    char *username = getenv("USER");
    strcpy(str1,username);
    strcat(str1,"@");
    strcat(str1,username);
    strcat(str1,":~");
    getcwd(cwd, sizeof(cwd));
    strcat(str1,cwd);
    strcat(str1,"$ ");
    
    buf = readline(str1);
    if (strlen(buf) != 0)
    {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } 
    else 
    
        return 1;
}


// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork(); 
 
    if (pid == -1) {
        printf("Failed forking child..\n");
        return;
    } else if (pid == 0){
    	if(!strcmp(parsed[0],"cat"))
    	{
    		strcat(buf,"/concat");
	        if (execvp(buf, parsed) < 0) {
	            printf("Could not execute command..\n");
	        }
	    }
	    else if(!strcmp(parsed[0],"mkdir"))
	    {
	    	strcat(buf,"/md");
	    	if (execvp(buf, parsed) < 0) {
	            printf("Could not execute command..\n");
	        }
	    }
	    else if(!strcmp(parsed[0],"top"))
	    {
	    	strcat(buf,"/top");
	    	if (execvp(buf, parsed) < 0) {

	            printf("Could not execute command..\n");
	        }
	    }
	    else
	    	if(execvp(parsed[0], parsed) < 0) printf("Could not execute command..\n");

        exit(0);
    } else {
        // waiting for child to terminate
        wait(NULL); 
        return;
    }
}


// Function where the redirected system command is executed
void execArgsRedirect(char **parsedArgs, char **parsedArgsRedirect,int flag)
{
	int pid,fd;
	
	if(flag==2)
	   fd = open(parsedArgsRedirect[0], O_WRONLY|O_TRUNC|O_CREAT, 0644);
	else
	   fd = open(parsedArgsRedirect[0], O_WRONLY|O_CREAT|O_APPEND, 0644);
	
	if (fd < 0) { 	
		printf("Could not open the file : %s\n",parsedArgsRedirect[0]); 
		return; 
	}
	switch (pid = fork()) {
	 	case -1: printf("Failed forking child..\n");
        		 return;
	        case 0:
	        	if (dup2(fd, 1) < 0) printf("Error duplicating file descriptor\n");
				else if(!strcmp(parsedArgs[0],"cat"))
	        	{
	        		strcat(buf,"/concat");
	    	   		if(execvp(buf, parsedArgs) < 0) printf("Could not execute command..\n");
	    	   	}
	    	   	else if(!strcmp(parsedArgs[0],"mkdir"))
	    	   	{
	    	   		strcat(buf,"/md");
	    	   		if(execvp(buf, parsedArgs) < 0) printf("Could not execute command..\n");
	    	   	}
	    	   	else if(!strcmp(parsedArgs[0],"top"))
			    {
			    	strcat(buf,"/top");
			    	if (execvp(buf, parsedArgs) < 0) {

			            printf("Could not execute command..\n");
			        }
			    }
	    	   	else
	    	   		if(execvp(parsedArgs[0], parsedArgs) < 0) printf("Could not execute command..\n");
	    	   	exit(0);
	  	default:
	  		// waiting for child to terminate
	  		wait(NULL);
	  		close(fd);
	    }
}


// Help command builtin
void openHelp()
{
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\nCopyright @ Subha Banerjee"
        "\nList of Commands supported:"
        "\n>cd"
        "\n>mkdir"
        "\n>cat"
        "\n>top"
        "\n>mkdir"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>Redirection handling"
        "\n>improper space handling");
 
    return;
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int NoOfOwnCmds = 3, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
 
    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
 
    for (i = 0; i < NoOfOwnCmds; i++) {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
            switchOwnArg = i + 1;
            break;
        }
    }
 
    switch (switchOwnArg) {
    case 1:
        printf("\nThank You for using this shell\n\n");
        exit(0);
    case 2:
		if(parsed[1] == NULL)
		{
			if(chdir("/") < 0)
			{
				int err = errno;
				if(err == ENOENT)
		        	printf("%s: No such file or Directory\n",parsed[1]);
		    	else if(err == ENOTDIR)
		    		printf("%s: Not a Directory\n",parsed[1]);
			}
		}
    	else if(!strcmp(parsed[1],"help"))
		{
			printf("Usage: cd <directory name>\n");
		}
		else if(chdir(parsed[1]) == -1)
		{
			int err = errno;
			if(err == ENOENT)
	        	printf("%s: No such file or Directory\n",parsed[1]);
	    	else if(err == ENOTDIR)
	    		printf("%s: Not a Directory\n",parsed[1]);
	    }
	    return 1;
    case 3:
        openHelp();
        return 1;
    default:
        break;
    }
 
    return 0;
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;
 
    for (i = 0; i < MAXARGS; i++) {
        parsed[i] = strsep(&str," ");
 
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int parseOp(char *str, char **strRedirect)
{
    int i;
    char *ch = strchr(str,'>');
    if(ch)
    {
	    for (i = 0; i < 3; i++) {
		strRedirect[i] = strsep(&str, ">");
	    }
	    
	    if(strlen(strRedirect[1]))
	 	return 1;  // Returning index for '>' operator
	    else
	    	return 2; //  Returning index for '>>' operator
    }
    else
        return 0; // returns zero if no Redirection operator is found.
}

int processString(char* str, char** parsed, char** parsedRedirect)
{
 
    char* strRedirect[3] = {NULL};
    int redirect = 0;
 
    redirect = parseOp(str, strRedirect);
    
    //printf("%s %s %s\n", strRedirect[0], strRedirect[1],strRedirect[2]);
 
    if (redirect) {
        parseSpace(strRedirect[0], parsed);
        parseSpace(strRedirect[redirect], parsedRedirect);
 
    } else {
 
        parseSpace(str, parsed);
    }
 
    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + redirect;
}

int main()
{
	char str[MAXCOM], *parsedArgs[MAXARGS];
	char* parsedArgsRedirect[MAXARGS];
    int execFlag = 0;
    getcwd(buf,sizeof(buf));
	
	while(1)
	{
	
		
		if(takeInput(str))
			continue;
			
		execFlag = processString(str, parsedArgs, parsedArgsRedirect);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a redirection.
		
		if (execFlag == 1)
		    execArgs(parsedArgs);
	 
		if (execFlag == 2 || execFlag == 3)
		    execArgsRedirect(parsedArgs, parsedArgsRedirect,execFlag);
	}

}
