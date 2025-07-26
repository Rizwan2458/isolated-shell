#include "limits.h"
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>

void apply_limits(void) {
    struct rlimit rl;

    // CPU time: 2 seconds
    rl.rlim_cur = rl.rlim_max = 2;
    if (setrlimit(RLIMIT_CPU, &rl) != 0) {
        perror("setrlimit CPU");
        exit(1);
    }

    // Virtual memory (address space): 64 MB
    rl.rlim_cur = rl.rlim_max = 64 * 1024 * 1024;
    if (setrlimit(RLIMIT_AS, &rl) != 0) {
        perror("setrlimit AS");
        exit(1);
    }
}
