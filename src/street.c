#include "street.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "object_loader.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Street* street_create(const char* textureFile)
{
    Street* street = malloc(sizeof(Street));

    
    float streetVertices[] = {
        // positions               // tex coords           // normals
        -10.0f,  0.0f, -1000.0f,   0.0f,  200.0f,       0.0f, 1.0f, 0.0f,// Top Left
         10.0f,  0.0f, -1000.0f,   1.0f,  200.0f,       0.0f, 1.0f, 0.0f,// Top Right
         10.0f,  0.0f,   100.0f,   1.0f,   0.0f,        0.0f, 1.0f, 0.0f,// Bottom Right
        -10.0f,  0.0f, -1000.0f,   0.0f,  200.0f,       0.0f, 1.0f, 0.0f,// Top Left
         10.0f,  0.0f,   100.0f,   1.0f,   0.0f,        0.0f, 1.0f, 0.0f,// Bottom Right
        -10.0f,  0.0f,   100.0f,   0.0f,   0.0f,        0.0f, 1.0f, 0.0f// Bottom Left
    };

    street->vertexCount = 6;

    DrawObject *obj = object_create_from_vertices(streetVertices, 6);
    street->textureID = obj->textureID;
    street->VAO = obj->VAO;
    street->VBO = obj->VBO;
    street->vertexCount = obj->vertexCount;
    // Load texture
    load_texture(textureFile, &street->textureID);

    glBindVertexArray(0);

    return street;
}
Street* pavement_create(const char* textureFile, bool left)
{
    Street* street = malloc(sizeof(Street));

    float side = left ? -1.0f : 1.0f;       // -1 = links, +1 = rechts
    float innerX = side * 10.0f;             // Kante, die an der Straße liegt
    float outerX = side * 30.0f;             // äußere Kante des Gehwegs (10 + 20 Versatz)
    float pavementY = 1.5f;
    float streetY   = 0.0f;

    float streetVertices[] = {
        // positions                       // tex coords        // normals
        // --- Pavement (Gehweg-Fläche) ---
        innerX, pavementY, -1000.0f,        0.0f, 200.0f,        0.0f, 1.0f, 0.0f, // Top Left
        outerX, pavementY, -1000.0f,        1.0f, 200.0f,        0.0f, 1.0f, 0.0f, // Top Right
        outerX, pavementY,   100.0f,        1.0f,   0.0f,        0.0f, 1.0f, 0.0f, // Bottom Right
        innerX, pavementY, -1000.0f,        0.0f, 200.0f,        0.0f, 1.0f, 0.0f, // Top Left
        outerX, pavementY,   100.0f,        1.0f,   0.0f,        0.0f, 1.0f, 0.0f, // Bottom Right
        innerX, pavementY,   100.0f,        0.0f,   0.0f,        0.0f, 1.0f, 0.0f, // Bottom Left

        // --- Curb (Stufe von Straße zu Gehweg) ---
        innerX, streetY,     100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Bottom Front
        innerX, pavementY,   100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Top Front
        innerX, pavementY,   -100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Top Back
        innerX, streetY,     100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Bottom Front
        innerX, pavementY,   -100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Top Back
        innerX, streetY,     -100.0f,        0.0f, 0.0f,         -side, 0.0f, 0.0f, // Bottom Back
    };

    street->vertexCount = (sizeof(streetVertices)/sizeof(float)) /8;

    DrawObject *obj = object_create_from_vertices(streetVertices, street->vertexCount);
    street->textureID = obj->textureID;
    street->VAO = obj->VAO;
    street->VBO = obj->VBO;
    street->vertexCount = obj->vertexCount;
    // Load texture
    load_texture(textureFile, &street->textureID);

    glBindVertexArray(0);

    return street;
}

void street_draw(Street* street, GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(street->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, street->textureID);
    glDrawArrays(GL_TRIANGLES, 0, street->vertexCount);
    glBindVertexArray(0);
}

void street_destroy(Street* street)
{
    glDeleteBuffers(1, &street->VBO);
    glDeleteVertexArrays(1, &street->VAO);
    glDeleteTextures(1, &street->textureID);
    free(street);
}
