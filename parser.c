#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parse_line(char *argv[]) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);
    if (nread <= 0) {
        free(line);
        return -1;
    }

    line[strcspn(line, "\n")] = '\0';
    int argc = 0;
    char *tok = strtok(line, " ");
    while (tok && argc < 63) {
        argv[argc++] = tok;
        tok = strtok(NULL, " ");
    }
    argv[argc] = NULL;
    return 0;
}
