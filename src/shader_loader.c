#include "shader_loader.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* filePath)
{
    FILE* file = fopen(filePath, "r");
    if (!file) {
        printf("Failed to open file: %s\n", filePath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(fileSize + 1);
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
        printf("Shader compilation failed: %s\n", infoLog);
        return 0;
    }

    return shader;
}

GLuint load_shader(const char* vertexPath, const char* fragmentPath)
{
    char* vertexSource = read_file(vertexPath);
    char* fragmentSource = read_file(fragmentPath);

    if (!vertexSource || !fragmentSource) {
        return 0;
    }

    GLuint vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Program linking failed: %s\n", infoLog);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vertexSource);
    free(fragmentSource);

    return program;
}
