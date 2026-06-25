#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "matrix.h"
#include "vector.h"
#include "shader_loader.h"
#include "skybox.h"
#include "street.h"

#include "../objects/loadObj.c"


Street *street;
Skybox *skybox;

GLuint basicShaderProgram;
GLuint skyboxShaderProgram;

// just float[16]
matrix4x4 view_rmo, projection_rmo, model_rmo;
matrix4x4 view, projection, model;

float fovy = 45.0f * 3.14159f / 180.0f;
Vec3 eye = {0.0f, 1.5f, 5.0f};

GLFWwindow* window;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    
    glViewport(0, 0, width, height);
}


void print_matrix(const matrix4x4 m)
{
    for (int i = 0; i < 4; i++) {
        printf("%f %f %f %f\n", m[i * 4], m[i * 4 + 1], m[i * 4 + 2], m[i * 4 + 3]);
    }
}

int init(void) {

    // glewExperimental = GL_TRUE is required to prevent crashes
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Load shaders
    basicShaderProgram = load_shader("shaders/basic.vert", "shaders/basic.frag");
    skyboxShaderProgram = load_shader("shaders/skybox.vert", "shaders/skybox.frag");

    if (!basicShaderProgram || !skyboxShaderProgram) {
        printf("Failed to load shaders\n");
        glfwTerminate();
        return -1;
    }

    // Create street (you need to provide a street texture)
    street = street_create("textures/road.png");

    // Create skybox (you need to provide 6 skybox textures)
    skybox = skybox_create (
        "textures/cloud_east.png",   // 1. Right (+X)
        "textures/cloud_west.png",   // 2. Left  (-X)
        "textures/cloud_up.png",     // 3. Top   (+Y)
        "textures/cloud_down.png",   // 4. Bottom(-Y)
        "textures/cloud_north.png",  // 5. Front (+Z or -Z depending on your camera)
        "textures/cloud_south.png" );  // 6. Back  (-Z or +Z)
    // Camera setup
    // eye = {0.0f, 1.5f, 5.0f};
    vector3 target = {0.0f, 1.5f, -10.0f};
    vector3 up = {0.0f, 1.0f, 0.0f}; // always 

    lookAt(view, eye, target, up);

    perspective(projection, fovy, 1280.0f / 720.0f, 0.1f, 100.0f);

    // Convert to row-major order for OpenGL
    cmo_to_rmo(view, view_rmo);
    cmo_to_rmo(projection, projection_rmo);

    // Set up matrices as identity for model
    for (int i = 0; i < 16; i++) {
        model[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }
    cmo_to_rmo(model, model_rmo);
}

void draw(void) {
            // Clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ==========================================
        // 1. DRAW STREET FIRST
        // ==========================================
        glUseProgram(basicShaderProgram);
        
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "model"),
                           1, GL_TRUE, (float*)model_rmo);
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "view"),
                           1, GL_TRUE, (float*)view_rmo);
        
        // Update projection matrix dynamically for window resizing
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float aspect = (float)width / (float)(height > 0 ? height : 1);
        perspective(projection, fovy, aspect, 0.1f, 100.0f);
        cmo_to_rmo(projection, projection_rmo);
        
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "projection"),
                           1, GL_TRUE, (float*)projection_rmo);

        // Set lighting uniforms
        vector3 lightPos = {5.0f, 5.0f, 5.0f};
        vector3 lightColor = {1.0f, 1.0f, 1.0f};
        glUniform3f(glGetUniformLocation(basicShaderProgram, "lightPos"),
                    lightPos[0], lightPos[1], lightPos[2]);
        glUniform3f(glGetUniformLocation(basicShaderProgram, "viewPos"),
                    eye[0], eye[1], eye[2]);
        glUniform3f(glGetUniformLocation(basicShaderProgram, "lightColor"),
                    lightColor[0], lightColor[1], lightColor[2]);
        glUniform1i(glGetUniformLocation(basicShaderProgram, "diffuseTexture"), 0);

        // Compute normal matrix
        matrix3x3 normalMatrix;
        compute_normal_matrix(normalMatrix, model_rmo);
        glUniformMatrix3fv(glGetUniformLocation(basicShaderProgram, "normalMatrix"),
                           1, GL_TRUE, (float*)normalMatrix);

        street_draw(street, basicShaderProgram);

        // ==========================================
        // 2. DRAW SKYBOX LAST
        // ==========================================
        glDepthFunc(GL_LEQUAL); // Trick the depth buffer
        glUseProgram(skyboxShaderProgram);
        
        // NOTICE: USING GL_TRUE HERE FOR THE SKYBOX MATRICES TOO!
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"),
                           1, GL_TRUE, (float*)projection_rmo);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"),
                           1, GL_TRUE, (float*)view_rmo);
                           
        glUniform1i(glGetUniformLocation(skyboxShaderProgram, "skybox"), 0);
        
        skybox_draw(skybox, skyboxShaderProgram);
        glDepthFunc(GL_LESS); // Restore default depth buffer behavior


}


int main(void)
{
    // 1. Start up GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Tell GLFW to use modern OpenGL (3.3 Core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the Window
    window = glfwCreateWindow(1280, 720, "Street with Skybox", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwMakeContextCurrent(window);
    
    // !! Set the framebuffer size callback to adjust viewport on window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (init() == -1) {
        printf("Init failed\n");
        return -1;
    }
    // Render loop
    while (!glfwWindowShouldClose(window)) {
        draw();
        // Swap buffers and check for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Cleanup
    street_destroy(street);
    skybox_destroy(skybox);
    glDeleteProgram(basicShaderProgram);
    glDeleteProgram(skyboxShaderProgram);
    glfwTerminate();
    return 0;
}
