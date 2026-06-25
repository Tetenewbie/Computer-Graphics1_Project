#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "camera.h"
#include "matrix.h"
#include "vector.h"
#include "shader_loader.h"
#include "skybox.h"
#include "street.h"
#include "object_loader.h"

#include "loadObj.h"


Street *street;
Skybox *skybox;
DrawObject *carObj = NULL;

float carZPosition = -50.0f;
float carAcceleration = 0.0f;


GLuint carTextureID;
GLuint basicShaderProgram;
GLuint skyboxShaderProgram;
GLuint vertexArrayObject;
    size_t carSize;

// just float[16]
matrix4x4 view, projection, model;

float fovy = 45.0f * 3.14159f / 180.0f;
Vec3 eye = {25.0f, 10.0f, 30.0f};
Vec3 target = {0.0f, 1.5f, -10.0f};
Vec3 up = {0.0f, 1.0f, 0.0f}; // always 


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
    
    lookAt(view, eye, target, up);

    perspective(projection, fovy, 1280.0f / 720.0f, 0.1f, 100.0f);


    // Set up matrices as identity for model
    identity(model);

    carObj = object_create("objects/car.obj");
    load_texture("textures/blue_metal_plate_diff_4k.jpg", &carObj->textureID);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

}

float clamp (float value, float min, float max) {
    if (value > max)
        return max;
    if (value < min)
        return min;
    return value;
}
void handleInputs(void) {
    printf("\rCar Z: %.4f", carZPosition);
    fflush(stdout);
    bool action = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // printf("wwww\n");
        carAcceleration = clamp(carAcceleration + 0.05, -2, 2);
        action = true;
    } 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // printf("wwww\n");
        carAcceleration = clamp(carAcceleration - 0.05, -2, 2);
        action = true;
    } 
    if (!action) {
        if (carAcceleration >= -0.01 && carAcceleration <= -0.01)
        carAcceleration = 0;
        if (carAcceleration > 0) {
            carAcceleration = clamp(carAcceleration - 0.01, 0, 2);
        } else if (carAcceleration < 0) {
            carAcceleration = clamp(carAcceleration + 0.01, -2, 0);
            
        } 
        carZPosition += carAcceleration;
    } else 
        carZPosition += carAcceleration;
}

void draw(void) {
            // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        handleInputs();
        target[2] = carZPosition;
        lookAt(view, eye, target, up);

        // ==========================================
        // 1. DRAW STREET FIRST
        // ==========================================
        glUseProgram(basicShaderProgram);
        
        identity(model);
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "model"),
                           1, GL_FALSE, (float*)model);
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "view"),
                           1, GL_FALSE, (float*)view);
        
        // Update projection matrix dynamically for window resizing
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        float aspect = (float)width / (float)(height > 0 ? height : 1);
        perspective(projection, fovy, aspect, 0.1f, 100.0f);
        
        glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "projection"),
                           1, GL_FALSE, (float*)projection);

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
        compute_normal_matrix(normalMatrix, model);
        glUniformMatrix3fv(glGetUniformLocation(basicShaderProgram, "normalMatrix"),
                           1, GL_FALSE, (float*)normalMatrix);

        street_draw(street, basicShaderProgram);

        // ==========================================
        // 2. DRAW SKYBOX LAST
        // ==========================================
        glDepthFunc(GL_LEQUAL); // Trick the depth buffer
        glUseProgram(skyboxShaderProgram);
        
        // NOTICE: USING GL_TRUE HERE FOR THE SKYBOX MATRICES TOO!
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "projection"),
                           1, GL_FALSE, (float*)projection);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShaderProgram, "view"),
                           1, GL_FALSE, (float*)view);
                           
        glUniform1i(glGetUniformLocation(skyboxShaderProgram, "skybox"), 0);
        
        skybox_draw(skybox, skyboxShaderProgram);
        glDepthFunc(GL_LESS); // Restore default depth buffer behavior

    
        // 
        // 3. DRAW CAR OBJECT 
        // 

        
        
        matrix4x4 worldMat;
        // matrix4x4 rotation;
        matrix4x4 translation;
        identity(model);
        identity(worldMat);
        identity(translation);

        translate(worldMat, worldMat, (Vec3) {-5.0f,0.1f,0.0f});
        translate(worldMat, worldMat, (Vec3) {0.0f,0.0f, carZPosition});
        scale(translation, translation, (Vec3) {0.5f,0.5f,0.5f});
        multiply(model, worldMat, translation);  // model = T * S
        memcpy(carObj->modelMat, model, sizeof(carObj->modelMat));
        
        object_draw(carObj, basicShaderProgram);
        // glUniform1i(glGetUniformLocation(basicShaderProgram, "diffuseTexture"), 0);
        // glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "model"),
        //                    1, GL_FALSE, (float*)model);


        //                    glDrawArrays(GL_TRIANGLES, 0, carSize);



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
