# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -std=c90 -g   -fsanitize=address
#
TARGET = assembler

SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)