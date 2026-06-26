CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter
LDFLAGS = -lglfw -lGLEW -lGL -lm

# Source and object files
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=obj/%.o)
TARGET = bin/street_scene

all: $(TARGET)

$(TARGET): $(OBJS) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

bin obj:
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf obj bin

.PHONY: all run clean