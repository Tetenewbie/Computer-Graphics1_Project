#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint textureID;
} Skybox;

Skybox* skybox_create(const char* right, const char* left, const char* top,
                       const char* bottom, const char* front, const char* back);
void skybox_draw(Skybox* skybox, GLuint shaderProgram);
void skybox_destroy(Skybox* skybox);

#endif
