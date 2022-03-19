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

.PHONY: analyze
analyze:
	valgrind  --undef-value-errors=no --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./$(TARGET) $(ARGS)