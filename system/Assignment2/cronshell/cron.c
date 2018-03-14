#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


void daemonize()
{
	pid_t pid;
	char dir[20] = "/home/";
    /* Fork off the parent process to disconnect from the terminal */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
    {
    	printf("fork failed!\n");
		// Return failure in exit status
        exit(EXIT_FAILURE);
    }

    /* Success: Let the parent terminate */
    if (pid > 0)
    {
    	printf("Parent Process id:%d\n",getpid());
		printf("process_id of first child process %d\n",pid);
        exit(EXIT_SUCCESS);
    }

    //set new session to make this child session leader
	if(setsid() < 0)
	{
		printf("Setting new session failed!\n");
		// Return failure
		exit(1);
	}

	/* Fork off the child process to disconnect from other peer precess of the parent preocess */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
    {
    	printf("fork failed!\n");
		// Return failure in exit status
        exit(EXIT_FAILURE);
    }

    /* Success: Let the child terminate */
    if (pid > 0)
    {
		printf("process_id of second child process %d\n",pid);
        exit(EXIT_SUCCESS);
    }

    //unmask the file mode
	umask(0);

	strcat(dir,getenv("USER"));
	// Change the current working directory to /home/<User directory name>
	chdir(dir);

	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

int parseMon(char *mon)
{
	if(!strcmp(mon,"jan") || !strcmp(mon,"Jan") || !strcmp(mon,"JAN"))
		return 0;
	else if(!strcmp(mon,"feb") || !strcmp(mon,"Feb") || !strcmp(mon,"FEB"))
		return 1;
	else if(!strcmp(mon,"mar") || !strcmp(mon,"Mar") || !strcmp(mon,"MAR"))
		return 2;
	else if(!strcmp(mon,"apr") || !strcmp(mon,"Apr") || !strcmp(mon,"APR"))
		return 3;
	else if(!strcmp(mon,"may") || !strcmp(mon,"May") || !strcmp(mon,"MAY"))
		return 4;
	else if(!strcmp(mon,"jun") || !strcmp(mon,"Jun") || !strcmp(mon,"JUN"))
		return 5;
	else if(!strcmp(mon,"jul") || !strcmp(mon,"Jul") || !strcmp(mon,"JUL"))
		return 6;
	else if(!strcmp(mon,"aug") || !strcmp(mon,"Aug") || !strcmp(mon,"AUG"))
		return 7;
	else if(!strcmp(mon,"sep") || !strcmp(mon,"Sep") || !strcmp(mon,"SEP"))
		return 8;
	else if(!strcmp(mon,"oct") || !strcmp(mon,"Oct") || !strcmp(mon,"OCT"))
		return 9;
	else if(!strcmp(mon,"nov") || !strcmp(mon,"Nov") || !strcmp(mon,"NOV"))
		return 10;
	else if(!strcmp(mon,"dec") || !strcmp(mon,"Dec") || !strcmp(mon,"DEC"))
		return 11;
}

int parseWOD(char *day)
{
	if(!strcmp(day,"sun") || !strcmp(day,"Sun") || !strcmp(day,"SUN"))
		return 0;
	else if(!strcmp(day,"mon") || !strcmp(day,"Mon") || !strcmp(day,"MON"))
		return 1;
	else if(!strcmp(day,"tue") || !strcmp(day,"Tue") || !strcmp(day,"TUE"))
		return 2;
	else if(!strcmp(day,"wed") || !strcmp(day,"Wed") || !strcmp(day,"WED"))
		return 3;
	else if(!strcmp(day,"thu") || !strcmp(day,"Thu") || !strcmp(day,"THU"))
		return 4;
	else if(!strcmp(day,"fri") || !strcmp(day,"Fri") || !strcmp(day,"FRI"))
		return 5;
	else if(!strcmp(day,"sat") || !strcmp(day,"Sat") || !strcmp(day,"SAT"))
		return 6;
}

// flag = 1 means month, flag = 2 means day of week and flag = 0 means others
bool check(int arg, char *str, int low, int high, int flag) 
{
	if(!strcmp(str,"*"))
		return 1;
	int arg1,arg2;
	char *llimit,*ulimit;
	char *ch1 = strchr(str,'-');
	char *ch2 = strchr(str,',');
	char *ch3 = strchr(str,'/'); 

	if(!ch1 && !ch2 && !ch3)
	{
		if(isalpha(*str))
		{
			if(flag == 1)
				arg1 = parseMon(str);
			else if(flag == 2)
				arg1 = parseWOD(str);
		}
		else
		{
			arg1 = atoi(str);
		}
		return (arg == arg1);
	}
	else if(ch1 && ch2)
	{
		char *left;
		while((left = strsep(&str,","))!=NULL)
		{
			llimit = strsep(&left,"-");
			ulimit = strsep(&left,"-");
			printf("%s %s\n", llimit,ulimit);

			if(isalpha(*llimit))
			{
				if(flag == 1)
				{
					arg1 = parseMon(llimit);
					arg2 = parseMon(ulimit);
				}
				else if(flag == 2)
				{
					arg1 = parseWOD(llimit);
					arg2 = parseWOD(ulimit);
				}
			}
			else
			{
				arg1 = atoi(llimit);
				arg2 = atoi(ulimit);
			}

			if(arg>=arg1 && arg<=arg2)
				return 1;
		}
	}
	else if(ch1 && ch3)
	{
		int i,arg3;
		char *left,*right;
		left = strsep(&str,"/");
		right = strsep(&str,"/");
		llimit = strsep(&left,"-");
		ulimit = strsep(&left,"-");
		arg3 = atoi(right);

		if(isalpha(*llimit))
		{
			if(flag == 1)
			{
				arg1 = parseMon(llimit);
				arg2 = parseMon(ulimit);
			}
			else if(flag == 2)
			{
				arg1 = parseWOD(llimit);
				arg2 = parseWOD(ulimit);
			}
		}
		else
		{
			arg1 = atoi(llimit);
			arg2 = atoi(ulimit);
		}
		if(arg1>=low && arg2<=high)
		{
			for(i=arg1;i<=arg2;i=i+arg3)
			{
				if(i == arg)
					return 1;
			}
		}

	}
	else if(ch1)
	{
		llimit = strsep(&str,"-");
		ulimit = strsep(&str,"-");
		//printf("%s %s\n", llimit,ulimit);

		if(isalpha(*llimit))
		{
			if(flag == 1)
			{
				arg1 = parseMon(llimit);
				arg2 = parseMon(ulimit);
			}
			else if(flag == 2)
			{
				arg1 = parseWOD(llimit);
				arg2 = parseWOD(ulimit);
			}
		}
		else
		{
			arg1 = atoi(llimit);
			arg2 = atoi(ulimit);
		}

		return (arg>=arg1 && arg<=arg2);
	}
	else if(ch2)
	{
		while((llimit = strsep(&str,","))!=NULL)
		{
			if(isalpha(*llimit))
			{
				if(flag == 1)
					arg1 = parseMon(llimit);
				else if(flag == 2)
					arg1 = parseWOD(llimit);
			}
			else
			{
				arg1 = atoi(llimit);
			}
			
			if(arg == arg1)
				return 1;
		}
	}
	else if(ch3)
	{
		int i;
		llimit = strsep(&str,"/");
		ulimit = strsep(&str,"/");

		if(isalpha(*llimit))
		{
			if(flag == 1)
				arg1 = parseMon(llimit);
			else if(flag == 2)
				arg1 = parseWOD(llimit);
		}
		else if(*llimit == '*')
			arg1 = low;
		else
			arg1 = atoi(llimit);

			arg2 = atoi(ulimit);

		for(i=arg1;i<=high;i+=arg2)
		{
			if(arg == i)
			return 1;
		}
	}

	return 0;
}

void matched(struct tm* tm)
{
	int ch;
	char line[200];
	FILE *fp = fopen("crontab","r");
	char err_message[200] = "echo \"could not open the file crontab\">> syslog";

	if(fp == NULL)
	{
		system(err_message);
		exit(1);
	}

	char minute[60],hour[60],dom[60],month[60],dow[60],command[100];

	//Reading crontab file
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch == '#')
			fgets(line,200,fp);
		else
			ungetc(ch,fp);

		fscanf(fp,"%s %s %s %s %s %[^\n]\n",minute,hour,dom,month,dow,command);
		//printf("%s %s %s %s %s %s\n",minute,hour,dom,month,dow,command);

		if (check(tm->tm_min,minute,0,59,0) && check(tm->tm_hour,hour,0,23,0) 
			&& check(tm->tm_mday,dom,1,31,0) && check(tm->tm_mon,month,0,11,1) 
			&& check(tm->tm_wday,dow,0,6,2))
		{
			//printf("matched");
			strcat(command," >> syslog");
			system(command);
		}
	}
	fclose(fp);
}

int main()
{
	// Daemonizing the process
	daemonize();
	// char dir[20] = "/home/";
	// strcat(dir,getenv("USER"));
	// chdir(dir);

	// setting timer for the daemon
	time_t end,start;
	start = end = time(NULL); 
	time_t seconds = 300; // Time span for running the daemon

	struct tm *tm;
	time_t t_now;

	while(seconds > (end - start))
	{
		time(&t_now);
		tm = localtime(&t_now);
		//printf("Min:%d Hour:%d DOM:%d Month:%d DOW:%d\n",tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_wday);
		matched(tm);
		sleep(60);

		end = time(0);
	}

}