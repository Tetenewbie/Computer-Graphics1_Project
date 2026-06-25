#include "object_loader.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "loadObj.h"

DrawObject* object_create(const char* objectFile)
{
    DrawObject* object = malloc(sizeof(DrawObject));
    identity(object->modelMat);
    float *carObj = loadObj((char *)objectFile, &object->vertexCount);
    GLuint triangleVertexBufferObject;
    glGenBuffers(1, &triangleVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject);
    // segments ist hier die Anzahl der Vertices. Jeder Vertex hat 8 Floats (3 Pos + 2 Tex + 3 Norm)
    glBufferData(GL_ARRAY_BUFFER, object->vertexCount * 8 * sizeof(float), carObj, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // create vertex array object
    glGenVertexArrays(1, &object->VAO);
    glBindVertexArray(object->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject);
    glVertexAttribPointer(
        0, // location aus vertex shader
        3, // wieviele elemente werden rausgenommen?
        GL_FLOAT,
        GL_FALSE,
        8*sizeof(GLfloat), // <--- stride MUSS hier 8 sein! (3+2+3)
        0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer( // textur
        1, // location aus vertex shader
        2, 
        GL_FLOAT,
        GL_FALSE,
        8*sizeof(GLfloat), // <--- stride MUSS hier 8 sein!
        (GLvoid*)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer( // normalenvektor
        2, // location aus vertex shader
        3, 
        GL_FLOAT,
        GL_FALSE,
        8*sizeof(GLfloat), // <--- stride MUSS hier 8 sein!
        (GLvoid*)(5 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);


    glBindVertexArray(0);


    free(carObj);
    carObj = 0;
    return object;

}

void load_texture(char *textureFile, GLuint *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // fixed min filter to use mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFile, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        printf("Failed to load car texture\n");
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void object_draw(DrawObject* object, GLuint shaderProgram)
{
    assert(object != NULL);
    
    
    
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
        1, GL_FALSE, (float*)object->modelMat);
    glBindVertexArray(object->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->textureID);
    glDrawArrays(GL_TRIANGLES, 0, object->vertexCount);
    glBindVertexArray(0);
}
