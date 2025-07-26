# isolated-shell
# Isolated Linux Shell

A custom Linux shell implemented in C that supports:

- Process creation and execution (`fork()`, `execvp()`, `waitpid()`)
- Resource limits with `setrlimit()` (CPU time, memory usage, etc.)
- Signal handling and background job support
- Command history
- Logging with timestamps

## Build Instructions

```bash
make
