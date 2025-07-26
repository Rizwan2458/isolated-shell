# Isolated Linux Shell

A custom Linux shell implemented in C that supports:

- Process creation and execution (`fork()`, `execvp()`, `waitpid()`)
- Resource limits with `setrlimit()` (CPU time, memory usage, etc.)
- Signal handling and background job support
- Background jobs using `&`
- Logging with timestamps to `log.txt`
- Built-in commands: `cd`, `pwd`, and `exit`

## Build Instructions

```bash
make

