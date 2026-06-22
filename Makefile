# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Wno-unused-parameter
LDFLAGS = -lglfw -lglew -lGL -lm

# Directories
SRC_DIR = src
SHADER_DIR = shaders
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/shader_loader.c \
       $(SRC_DIR)/street.c \
       $(SRC_DIR)/skybox.c \
       $(SRC_DIR)/camera.c \
       $(SRC_DIR)/matrix.c \
       $(SRC_DIR)/vector.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output executable
TARGET = $(BIN_DIR)/street_scene

# Shader files (for dependency tracking)
SHADERS = $(SHADER_DIR)/basic.vert \
          $(SHADER_DIR)/basic.frag \
          $(SHADER_DIR)/skybox.vert \
          $(SHADER_DIR)/skybox.frag

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(SHADER_DIR)
	@mkdir -p textures

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	@echo "Clean complete"

# Full clean (removes everything including binaries)
distclean: clean
	@echo "Distribution clean complete"

# Check for shader files
check-shaders:
	@echo "Checking shader files..."
	@for shader in $(SHADERS); do \
		if [ -f "$$shader" ]; then \
			echo "✓ $$shader"; \
		else \
			echo "✗ $$shader (MISSING)"; \
		fi \
	done

# Check for required libraries
check-libs:
	@echo "Checking for required libraries..."
	@pkg-config --exists glfw3 && echo "✓ GLFW3" || echo "✗ GLFW3 (MISSING)"
	@pkg-config --exists glew && echo "✓ GLEW" || echo "✗ GLEW (MISSING)"
	@pkg-config --exists gl && echo "✓ OpenGL" || echo "✗ OpenGL (MISSING)"

# Help target
help:
	@echo "Available targets:"
	@echo "  make              - Build the project"
	@echo "  make run          - Build and run the program"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make distclean    - Remove all generated files"
	@echo "  make check-shaders - Verify shader files exist"
	@echo "  make check-libs   - Check for required libraries"
	@echo "  make help         - Show this help message"

# Phony targets (not real files)
.PHONY: all run clean distclean check-shaders check-libs help directories
