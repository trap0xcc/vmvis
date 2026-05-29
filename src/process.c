
#include "process.h"

void process_init(process_info *pi) { pthread_mutex_init(&pi->mu, nullptr); }

pid_t process_get_pid(process_info *pi) {
  pthread_mutex_lock(&pi->mu);
  auto pid = pi->pid;
  pthread_mutex_unlock(&pi->mu);
  return pid;
}
