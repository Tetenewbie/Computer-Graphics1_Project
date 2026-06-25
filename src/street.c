#include "street.h"
#include <stdlib.h>
#include <stdio.h>
#include "object_loader.h"
// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Street* street_create(const char* textureFile)
{
    Street* street = malloc(sizeof(Street));

    

    // care, in basic.vert ist das layout anders (positions 0, texcoords 1, normals 2)
    float streetVertices[] = {
        // positions               // normals          // tex coords
        -10.0f,  0.0f, -1000.0f,   0.0f, 1.0f, 0.0f,   0.0f,  200.0f,  // Top Left
         10.0f,  0.0f, -1000.0f,   0.0f, 1.0f, 0.0f,   1.0f,  200.0f,  // Top Right
         10.0f,  0.0f,   100.0f,   0.0f, 1.0f, 0.0f,   1.0f,   0.0f,  // Bottom Right

        -10.0f,  0.0f, -1000.0f,   0.0f, 1.0f, 0.0f,   0.0f,  200.0f,  // Top Left
         10.0f,  0.0f,   100.0f,   0.0f, 1.0f, 0.0f,   1.0f,   0.0f,  // Bottom Right
        -10.0f,  0.0f,   100.0f,   0.0f, 1.0f, 0.0f,   0.0f,   0.0f   // Bottom Left
    };

    street->vertexCount = 6;

    glGenVertexArrays(1, &street->VAO);
    glGenBuffers(1, &street->VBO);

    glBindVertexArray(street->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, street->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(streetVertices), streetVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
