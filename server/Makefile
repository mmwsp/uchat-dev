CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinc
LDFLAGS = -ljson-c

SRC_DIR = src
INC_DIR = inc

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(SRCS))

TARGET = uchat_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
