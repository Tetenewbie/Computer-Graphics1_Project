#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include <GL/glew.h>

GLuint load_shader(const char* vertexPath, const char* fragmentPath);
GLuint compile_shader(const char* source, GLenum shaderType);

#endif
