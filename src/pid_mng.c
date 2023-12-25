#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "pid_mng.h"

void store_pid(const char *device, pid_t pid)
{
    FILE *fp;
    int ip;
    fp = fopen("/usr/local/share/disabld/data", "w+");
    if (fp == NULL)
    {
        perror("Cannot open /usr/local/share/disabld/data");
        exit(EXIT_FAILURE);
    }
    else
    {
        if (ip = fprintf(fp, "%s %d", device, pid) == 0)
        {
            perror("Cannot write file");
            exit(EXIT_FAILURE);
        }
        printf("\n%d\n", ip);
    }
    fclose(fp);
}

pid_t retrieve_pid(const char *device)
{
    char cur_dev[20];
    int pid, retpid;
    FILE *fp = fopen("/etc/disabld/data", "r");
    if (fp == NULL)
    {
        perror("Cannot open /usr/local/share/disabld/data");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp))
    {
        fscanf(fp, "%s %d", cur_dev, pid);
        if (strcmp(cur_dev, device) == 0)
        {
            retpid = pid;
            break;
        }
    }
    fclose(fp);
    if (retpid)
    {
        return retpid;
    }
    else
    {
        perror("Cannot find running device");
        exit(EXIT_FAILURE);
    }
}