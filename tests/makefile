# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter
LIBS = -lm

# Find all .c files in the tests folder and the src folder
TEST_SRCS = $(wildcard *.c)
SRC_SRCS = $(wildcard ../src/*.c)

TARGET = run_tests

all: $(TARGET)


$(TARGET): $(TEST_SRCS) $(SRC_SRCS)
	$(CC) $(CFLAGS) $(TEST_SRCS) $(SRC_SRCS) -o $(TARGET) $(LIBS)


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)
