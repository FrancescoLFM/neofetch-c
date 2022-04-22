SH = /bin/bash
CC = gcc
CFLAGS = -Wall -Wextra
CFLAGS += -g
CFLAGS += -I.
CFLAGS += -Fanalyzer

OBJ = main.o src/cpu.o src/utils.o src/distro.o src/neofetch.o
TARGET = main

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm $(OBJ)
	rm $(TARGET)

.PHONY: run
run:
	@./$(TARGET)

.PHONY: debug
debug:
	gdb -q $(TARGET)

.PHONY: analyze
analyze:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./$(TARGET) $(ARGS)
