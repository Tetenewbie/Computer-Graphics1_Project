#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <time.h>

#include "camera.h"
#include "matrix.h"
#include "vector.h"
#include "shader_loader.h"
#include "skybox.h"
#include "street.h"
#include "object_loader.h"

#include "loadObj.h"


Street *street;
Street *pavement[2];
Skybox *skybox;
DrawObject *carObj = NULL;
// DrawObject *cloudObj = NULL;
DrawObject *clouds[16] = {NULL};

DrawObject *otherObjects[4] = {NULL};

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
Vec3 eye = {25.0f, 10.0f, 00.0f};
Vec3 target = {0.0f, 1.5f, -10.0f}; // z coordinate gets changed to aim at car
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
    pavement[0] = pavement_create("textures/brick_pavement_03_diff_1k.jpg", true);
    pavement[1] = pavement_create("textures/brick_pavement_03_diff_1k.jpg", false);

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
    GLuint whitenoiseId;
    load_texture("textures/whitenoise.png", &whitenoiseId);
        srand(time(NULL));

    for (int i = 0; i<16; i++) {
        clouds[i] = object_create("objects/cloud.obj");
        clouds[i]->textureID = whitenoiseId;
        identity(model);
        if (i < 8)
            translate(model, model, (Vec3){-7.0f+1.5*i, 0.5f, -90});
        else
            translate(model, model, (Vec3){-7.0f+1.5*(i-8), 0.5f, 90});
            
        scale(model, model, (Vec3){2.5f, 2.5f, 2.5f});
        rotatex(model, model, rand());
        // rotatey(model, model, rand());
        rotatez(model, model, rand());
        memcpy(clouds[i]->modelMat, model, sizeof(clouds[i]->modelMat));
    // -60, 100
    }

    matrix4x4 tableModelMat;
    
    identity(tableModelMat);
    // table
    otherObjects[0] = object_create("objects/wooden_stool.obj");
    load_texture("textures/folding_wooden_stool_diff_1k.jpg", &otherObjects[0]->textureID);
    
    translate(tableModelMat, tableModelMat, (Vec3){-15.0f,1.5f,0.0f});
    scale(tableModelMat, tableModelMat, (Vec3) {5,5,5});
    memcpy(otherObjects[0]->modelMat, tableModelMat, sizeof(otherObjects[0]->modelMat));
    
    // chair
    otherObjects[1] = object_create("objects/wooden_chair.obj");
    load_texture("textures/painted_wooden_chair_01_diff_1k.jpg", &otherObjects[1]->textureID);
    
    // translate chair model matrix from table model mat, so we can move chair relative to table
    translate(model, tableModelMat, (Vec3){0.0f,0.0f,-1.0f});
    // scale(model, model, (Vec3) {5,5,5});
    memcpy(otherObjects[1]->modelMat, model, sizeof(otherObjects[1]->modelMat));
    
    // chair 2
    otherObjects[2] = object_create("objects/wooden_chair.obj");
    load_texture("textures/painted_wooden_chair_01_diff_1k.jpg", &otherObjects[2]->textureID);
    
    translate(model, tableModelMat, (Vec3){0.0f,0.0f,1.0f});
    rotatey(model, model, degrees_to_radians(180));
    // scale(model, model, (Vec3) {5,5,5});
    memcpy(otherObjects[2]->modelMat, model, sizeof(otherObjects[2]->modelMat));
  
    // teapot
    otherObjects[3] = object_create("objects/teapot.obj");
    load_texture("textures/painted_wooden_chair_01_diff_1k.jpg", &otherObjects[3]->textureID);
    translate(model, tableModelMat, (Vec3){0.25f,0.55f,0.0f});
    scale(model, model, (Vec3){0.05f,0.05f,0.05f});
    rotatey(model, model, degrees_to_radians(180));
    // scale(model, model, (Vec3) {5,5,5});
    memcpy(otherObjects[3]->modelMat, model, sizeof(otherObjects[3]->modelMat));
  
    
    



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
        vector3 lightPos = {5.0f, 15.0f, 5.0f};
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
        
        street_draw(pavement[0], basicShaderProgram);
        street_draw(pavement[1], basicShaderProgram);

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

        
        identity(model);
        for (int i = 0; i < sizeof(otherObjects)/sizeof(DrawObject *); i++) {
            if (otherObjects[i] == NULL)
                continue;
            object_draw(otherObjects[i], basicShaderProgram);
        }

        // 
        // 5. TRANSPARENT OBJECTS
        //
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1f(glGetUniformLocation(basicShaderProgram, "alpha"), 0.2);
        for (int i = 0; i < 16; i++) {
            object_draw(clouds[i], basicShaderProgram);
        }

        glUniform1f(glGetUniformLocation(basicShaderProgram, "alpha"), 1);
        glDisable(GL_BLEND);
        
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
