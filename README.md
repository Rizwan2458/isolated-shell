# Isolated Linux Shell

This is a custom Linux shell implemented in C that supports process creation, basic job control, I/O redirection, and resource limits. The project was built as a system programming exercise to understand low-level Unix process control and resource management.

---

## Features

- Built-in commands: `cd`, `pwd`, `exit`
- Execute external programs using `fork()` and `execvp()`
- Background job execution using `&`
- Input and output redirection using `<` and `>`
- CPU and memory resource limits enforced via `setrlimit()`
- Logging of terminated child processes with timing information to `log.txt`
- Signal handling for clean background process management
- Dynamic shell prompt displaying the current working directory

---

## Build Instructions

To compile the shell, run:

```
make
```

This will generate an executable file named `shell`.

---

## Running the Shell

To start the shell, run:

```
./shell
```

Once running, you’ll see a prompt like:

```
[/home/user/isolated-shell]$
```

You can enter commands as you would in a typical Unix shell. For example:

```
ls
cd ..
pwd
cat input.txt > output.txt
./long_task &
```

To exit the shell, type:

```
exit
```

---

## Project Structure

```
isolated-shell/
├── shell.c        // Main shell implementation
├── parser.c       // Command parsing logic
├── parser.h       // Header for parser
├── limits.c       // Code to apply CPU and memory limits
├── limits.h       // Header for limits
├── logger.c       // Code to log process exits
├── logger.h       // Header for logger
├── Makefile       // Build instructions
├── README.md      // Project documentation
├── .gitignore     // Files to ignore in version control
├── log.txt        // Runtime log file (auto-generated)
```

---

## Clean Build

To remove compiled binaries and object files:

```
make clean
```

---
