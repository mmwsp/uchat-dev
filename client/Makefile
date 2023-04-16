CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinc -I/opt/homebrew/Cellar/json-c/0.16/include/ -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -ljson-c -lsqlite3
SRC_DIR = src
INC_DIR = inc

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(SRCS))

TARGET = uchat_client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
