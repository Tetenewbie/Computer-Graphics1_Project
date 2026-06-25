#ifndef STREET_H
#define STREET_H

#include <GL/glew.h>
#include <stdbool.h>


typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint textureID;
    int vertexCount;
} Street;

Street* street_create(const char* textureFile);
Street* pavement_create(const char* textureFile, bool left);
void street_draw(Street* street, GLuint shaderProgram);
void street_destroy(Street* street);

#endif
