#ifndef PID_MNG_H
#define PID_MNG_H

#include <sys/types.h>

void store_pid(char*, pid_t);
pid_t retrieve_pid(char *);

#endif