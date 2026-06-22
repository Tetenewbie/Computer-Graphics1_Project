#ifndef STREET_H
#define STREET_H

#include <GL/glew.h>

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint textureID;
    int vertexCount;
} Street;

Street* street_create(const char* textureFile);
void street_draw(Street* street, GLuint shaderProgram);
void street_destroy(Street* street);

#endif
