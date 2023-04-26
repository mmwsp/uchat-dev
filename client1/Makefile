CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinc -I/usr/local/opt/openssl/include
LDFLAGS = -L/opt/homebrew/lib -L/usr/local/opt/openssl/lib -ljson-c -lsqlite3 -lssl -lcrypto
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
