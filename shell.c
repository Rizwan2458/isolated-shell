#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <errno.h>

#include "parser.h"
#include "limits.h"
#include "logger.h"

// Handle built-in commands like cd, pwd, and exit
int handle_builtin(char *argv[]) {
    if (!argv[0]) return 0;

    if (strcmp(argv[0], "cd") == 0) {
        char *target = argv[1] ? argv[1] : getenv("HOME");
        if (chdir(target) != 0) perror("cd");
        return 1;
    }

    if (strcmp(argv[0], "pwd") == 0) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)))
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 1;
    }

    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }

    return 0;
}

// Reap background processes and log them
void sigchld_handler(int sig) {
    (void)sig;  // silence unused parameter warning

    int saved_errno = errno;
    pid_t pid;
    int status;
    struct rusage usage;

    // Reap all terminated background children
    while ((pid = wait4(-1, &status, WNOHANG, &usage)) > 0) {
        printf("\n[bg done] pid=%d ", pid);
        if (WIFEXITED(status))
            printf("exit=%d", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("signal=%d", WTERMSIG(status));
        printf("\n");
        fflush(stdout);

        log_exit(pid, status, &usage);
    }

    errno = saved_errno;
}

int main(void) {
    // Ignore Ctrl+C
    signal(SIGINT, SIG_IGN);

    // Register SIGCHLD handler
    struct sigaction sa = {0};
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    while (1) {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd");
            strcpy(cwd, "?");
        }

        printf("[%s]$ ", cwd);
        fflush(stdout);

        char *argv[64];
        char *outfile = NULL;
        char *infile = NULL;
        bool background = false;

        if (parse_line(argv) == -1)
            break;

        if (handle_builtin(argv))
            continue;

        // Check for input/output redirection
        for (int i = 0; argv[i]; i++) {
            if (strcmp(argv[i], ">") == 0 && argv[i + 1]) {
                outfile = argv[i + 1];
                argv[i] = NULL;
                break;
            }
            if (strcmp(argv[i], "<") == 0 && argv[i + 1]) {
                infile = argv[i + 1];
                argv[i] = NULL;
                break;
            }
        }

        // Check for background execution symbol &
        int argc = 0;
        while (argv[argc]) argc++;
        if (argc > 0 && strcmp(argv[argc - 1], "&") == 0) {
            background = true;
            argv[argc - 1] = NULL;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child process

            apply_limits();

            if (infile) {
                int fd = open(infile, O_RDONLY);
                if (fd < 0) { perror("open infile"); exit(1); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            if (outfile) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) { perror("open outfile"); exit(1); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        } else {
            // Parent process

            if (background) {
                printf("[bg] pid %d\n", pid);
            } else {
                int status;
                struct rusage usage;
                wait4(pid, &status, 0, &usage);
                log_exit(pid, status, &usage);
            }
        }
    }

    return 0;
}