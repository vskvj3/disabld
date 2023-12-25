#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

void store_pid(char *device, pid_t pid)
{
    FILE *fp = open("/etc/disabld/data");
    fprintf(fp, "%s %d", device, pid);
}

pid_t retrieve_pid(char *device)
{
    char cur_dev[20];
    int pid, retpid=NULL;
    FILE *fp = open("/etc/disabld/data");
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", cur_dev, pid);
        if(strcmp(cur_dev, device) == 0){
            retpid = pid;
            break;
        }
    }
    if(retpid){
        return retpid;
    } else {
        perror("Cannot find running device");
        exit(EXIT_FAILURE);
    }
}