CC=gcc
CFLAGS= -Iinc -Wall -Wextra -Werror -Wpedantic -std=c11 -pthread
HEAD=inc/server.h
OBJ=src/main.o

%.o: %.c $(HEAD)
	$(CC) -c -o $@ $< $(CFLAGS)

uchat_server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) uchat_server
