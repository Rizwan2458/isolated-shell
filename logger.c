#include "logger.h"
#include <stdio.h>
#include <time.h>
#include <sys/wait.h>

void log_exit(pid_t pid, int status, struct rusage *usage) {
    time_t now = time(NULL);
    FILE *f = fopen("shell.log", "a");
    if (!f) return;

    fprintf(f, "%ld: pid=%d ", now, pid);
    if (WIFEXITED(status))
        fprintf(f, "exit=%d ", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        fprintf(f, "signal=%d ", WTERMSIG(status));

    double ut = usage->ru_utime.tv_sec + usage->ru_utime.tv_usec/1e6;
    double st = usage->ru_stime.tv_sec + usage->ru_stime.tv_usec/1e6;
    fprintf(f, "user=%.3f sys=%.3f\n", ut, st);

    fclose(f);
}
