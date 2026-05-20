#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// #include "texture_loader.h" // 

int main(void) {
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Roulette Table", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    
    // glewExperimental = GL_TRUE is required to prevent crashes 
    glewExperimental = GL_TRUE; 
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    
    while (!glfwWindowShouldClose(window)) {
        
        // Paint the background casino green
        glClearColor(0.1f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap buffers and check for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean 
    glfwTerminate();
    return 0;
}