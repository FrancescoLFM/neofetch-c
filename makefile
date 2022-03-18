SH = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS += -g

OBJ = main.o
TARGET = main

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm $(OBJ)
	rm $(TARGET)

.PHONY: run
run:
	./$(TARGET)

.PHONY: debug
debug:
	gdb -q $(TARGET)