#include "street.h"
#include <stdlib.h>
#include <stdio.h>

// #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Street* street_create(const char* textureFile)
{
    Street* street = malloc(sizeof(Street));

    
    float streetVertices[] = {
        // positions             // normals           // tex coords
        -10.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,   0.0f,  20.0f,  // Top Left
         10.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,   1.0f,  20.0f,  // Top Right
         10.0f,  0.0f,   10.0f,   0.0f, 1.0f, 0.0f,   1.0f,   0.0f,  // Bottom Right

        -10.0f,  0.0f, -100.0f,   0.0f, 1.0f, 0.0f,   0.0f,  20.0f,  // Top Left
         10.0f,  0.0f,   10.0f,   0.0f, 1.0f, 0.0f,   1.0f,   0.0f,  // Bottom Right
        -10.0f,  0.0f,   10.0f,   0.0f, 1.0f, 0.0f,   0.0f,   0.0f   // Bottom Left
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

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load texture
    glGenTextures(1, &street->textureID);
    glBindTexture(GL_TEXTURE_2D, street->textureID);

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
        printf("Failed to load street texture: %s\n", textureFile);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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
