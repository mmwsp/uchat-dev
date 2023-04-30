CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -pthread

OBJDIR = obj
SRCDIR = src
UCHAT = ../uchat_server

SRC = $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
OBJS = $(OBJDIR)/*.o

LIB_FLAGS =  -ljson-c -lsqlite3
all: $(UCHAT)

$(UCHAT):
	mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRC)
	mv *.o $(OBJDIR)
	$(CC) $(CFLAGS) -o $(UCHAT) $(OBJS) $(LIB_FLAGS)

clean:
	rm -f $(OBJS)
	rm -df $(OBJDIR)
	
uninstall:
	make clean
	rm -f $(UCHAT)

reinstall:
	make uninstall
	make all
