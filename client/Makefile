CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinc
LDFLAGS = -lm -ljson-c


SRC_DIR = src
INC_DIR = inc
CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/json_handle.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

.PHONY: all clean

all: client

client: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/client.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f client $(CLIENT_OBJ)
