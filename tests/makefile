# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter
LIBS = -lm

# Find all .c files in the tests folder and the src folder
TEST_SRCS = $(wildcard *.c)
SRC_SRCS = $(filter-out ../src/main.c ../src/skybox.c ../src/street.c ../src/shader_loader.c ../src/stb_image.c, $(wildcard ../src/*.c))

TARGET = run_tests

all: $(TARGET)


$(TARGET): $(TEST_SRCS) $(SRC_SRCS)
	$(CC) $(CFLAGS) $(TEST_SRCS) $(SRC_SRCS) -o $(TARGET) $(LIBS)


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)
