
#include "process.h"

void process_init(process_t *proc) { pthread_mutex_init(&proc->mu, NULL); }

pid_t process_get_pid(process_t *proc) {
  pthread_mutex_lock(&proc->mu);
  auto pid = proc->pid;
  pthread_mutex_unlock(&proc->mu);
  return pid;
}
