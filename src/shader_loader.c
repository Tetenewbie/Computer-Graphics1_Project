#include "shader_loader.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filePath)
{
    FILE* file = fopen(filePath, "r");
    if (!file) {
        printf("✗ Failed to open file: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed for file: %s\n", filePath);
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    return buffer;
}

GLuint compile_shader(const char* source, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        const char* shaderType_str = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
        printf("✗ %s Shader compilation failed:\n%s\n", shaderType_str, infoLog);
        return 0;
    }

    return shader;
}

GLuint load_shader(const char* vertexPath, const char* fragmentPath)
{
    printf("Loading shaders: %s, %s\n", vertexPath, fragmentPath);

    char* vertexSource = read_file(vertexPath);
    char* fragmentSource = read_file(fragmentPath);

    if (!vertexSource || !fragmentSource) {
        printf("✗ Failed to read shader files\n");
        if (vertexSource) free(vertexSource);
        if (fragmentSource) free(fragmentSource);
        return 0;
    }

    GLuint vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    if (!vertexShader) {
        printf("✗ Failed to compile vertex shader\n");
        free(vertexSource);
        free(fragmentSource);
        return 0;
    }

    GLuint fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        printf("✗ Failed to compile fragment shader\n");
        glDeleteShader(vertexShader);
        free(vertexSource);
        free(fragmentSource);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("✗ Program linking failed:\n%s\n", infoLog);
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        free(vertexSource);
        free(fragmentSource);
        return 0;
    }

    printf("✓ Shaders loaded and linked successfully\n");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexSource);
    free(fragmentSource);

    return program;
}
