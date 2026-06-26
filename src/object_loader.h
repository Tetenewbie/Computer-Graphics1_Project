#ifndef DRAWOBJ_H
#define DRAWOBJ_H

#include <GL/glew.h>
#include "matrix.h"
#include "loadObj.h"

typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint textureID;
    size_t vertexCount;
    matrix4x4 modelMat;
    Vec2 texScale;
} DrawObject;

DrawObject* object_create(const char* textureFile);
void load_texture(char *textureFile, GLuint *texture);
void object_draw(DrawObject* street, GLuint shaderProgram);
DrawObject *object_create_from_vertices(float *carObj, size_t vertexCount);
#endif
