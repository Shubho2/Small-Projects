#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h> // Directory read,close etc
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>


void print_process_status(long int process_no) 
{
    char path[40], line[100], *p,property[20];
    FILE* status_fp;
    int val,pid;
    char size[10];
    // Since fopen takes const path(no variable allowed in the path) we first create path with snprintf
    snprintf(path, 40, "/proc/%ld/status", process_no);

    status_fp = fopen(path, "r");
    if(!status_fp)
        return;

    while(fgets(line, 100, status_fp)) {

        /*
        * int strncmp(const char *s1, const char *s2, size_t n);
        * strncmp compares first n bytes of s1 and s2 if equal return 0
        */
        if(strncmp(line, "Pid:", 4) == 0){
            // Ignoring "Pid:" and whitespace
            p = line + 5;
            while(isspace(*p)) ++p;
            // printf prints strating from p till \0 found
            printf("\t");
            while(*p != '\n')
                printf("%c", *(p++));
            printf("\t\t");
        }
        else if(strncmp(line, "VmSize:", 7) == 0){
            p = line + 8;
            while(isspace(*p)) ++p;
            while(*p != '\n')
                printf("%c", *(p++));
            printf("\t\t");
        } 
        else if(strncmp(line, "VmRSS:", 6) == 0){
            p = line + 7;
            while(isspace(*p)) ++p;
            while(*p != '\n')
                printf("%c", *(p++));
            printf("\t\t");
        }
    }
    fseek(status_fp,0,SEEK_SET);
    while(fgets(line, 100, status_fp)){
        if(strncmp(line, "State:", 6) == 0){
            p = line + 7;
            while(isspace(*p)) ++p;
            while(*p != '\n')
                printf("%c", *(p++));
            printf("\t");
        }
    }
    fseek(status_fp,0,SEEK_SET);
    while(fgets(line, 100, status_fp)){
        if(strncmp(line, "Name:", 5) == 0){
            p = line + 6;
            while(isspace(*p)) ++p;
            while(*p != '\n')
                printf("%c", *(p++));
            printf("\n");
        }
    }

    fclose(status_fp);
}



int main(int argc, char *argv[])
{


    int val;
    int total,free,buff,used;
    char size[10],line[1024];

    /*
    * Print current memory usage
    */
    FILE *fp = fopen("/proc/meminfo","r");
    while(fscanf(fp,"%s%d%s",line,&val,size) != EOF){
        
        if (strcmp(line,"MemTotal:") == 0)
        {
            total = val;
            printf("%s Mem :    ",size);
            printf("%d total    ", val);
        }
        else if (strcmp(line,"MemFree:") == 0)
        {
            free = val;
            printf("%d free    ", val);
        }
        else if (strcmp(line,"Cached:") == 0)
        {
            buff = val;
            used = total - free - buff;
            printf("%d used    ", used);
            printf("%d buff/cached\n", val);
        }
    }
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp,"%s%d%s",line,&val,size) != EOF){
        if (strcmp(line,"SwapTotal:") == 0)
        {
            printf("%s Swap :    ",size);
            printf("%d total    ", val);
            total = val;
        }
        else if (strcmp(line,"SwapFree:") == 0)
        {
            free = val;
            printf("%d free    ", val);
        }
    }
    printf("%d  used    ", total - free);
    fseek(fp,0,SEEK_SET);
    while(fscanf(fp,"%s%d",line,&val) != EOF){
        if (strcmp(line,"MemAvailable:") == 0)
        {
            printf("%d  avail mem \n", val);
        }
    }
    fclose(fp);

    /*
    * Print current process information
    * 

        struct dirent {
            ino_t          d_ino;       // inode number 
            off_t          d_off;       // not an offset; see NOTES 
            unsigned short d_reclen;    // length of this record 
            unsigned char  d_type;      // type of file; not supported by all filesystem types 
            char           d_name[256]; // filename 
        };

    */
    
    DIR* proc_dir = opendir("/proc");
    struct dirent* dir_entry;
    long int process_no;

    if(proc_dir == NULL) {
        perror("opendir(/proc)");
        return 0;
    }
    printf("\n\tPID\t\tVIRT\t\t\tRES\t\tSLEEP/RUNNING\tCOMMAND\n\n");
    while(dir_entry = readdir(proc_dir)) {

        // if directory name not a number don't consider
        if(!isdigit(*dir_entry->d_name))
            continue;

        process_no = strtol(dir_entry->d_name, NULL, 10);

        // Read process status from status file of the corresponding process_no
        print_process_status(process_no);
    }
    // Finally close the /proc directory
    closedir(proc_dir);     

    return 0;     
}