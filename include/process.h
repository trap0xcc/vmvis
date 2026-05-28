
#ifndef _PROCESS_H
#define _PROCESS_H

#include <pthread.h>
#include <sys/types.h>

typedef struct {
  pthread_mutex_t mu;
  pid_t pid;
} process_t;

void process_init(process_t *proc);
pid_t process_get_pid(process_t *proc);

#endif
