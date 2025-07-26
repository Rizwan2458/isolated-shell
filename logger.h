#ifndef LOGGER_H
#define LOGGER_H
#include <sys/resource.h>
#include <sys/types.h>
void log_exit(pid_t pid, int status, struct rusage *usage);
#endif
