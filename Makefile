CC = gcc
CFLAGS = -Wall -Wextra -O2

all: shell

shell: shell.o parser.o limits.o logger.o
	$(CC) $(CFLAGS) -o shell shell.o parser.o limits.o logger.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o shell