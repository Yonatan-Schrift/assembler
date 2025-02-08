CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c90 -g
GLOBAL_DEPS = globals.h # Global dependencies

TARGET = main

SRCS := $(wildcard ./src/*.c)
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

%.o: %.c $(GLOBAL_DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f $(TARGET) $(OBJECTS)