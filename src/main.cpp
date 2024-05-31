#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>

// hello input

// we should also define a callback function for if/when the user changes the width/height of the window
// GLFW can do this for us
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit(); // initialise GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // configure with `glfwWindowHint`
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to initialise window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD is used for locating OpenGL function pointers, so before we start using OpenGL directly we initialise it here
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // tell OpenGL our window width and height
    glViewport(0, 0, 800, 600);

    // see framebuffer_size_callback()
    // we just need to tell GLFW we want it to use this function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)) { // self-explanatory - this is our render loop :)
        processInput(window);

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f); // state setting function
        glClear(GL_COLOR_BUFFER_BIT);         // state using function

        glfwSwapBuffers(window);            // double buffered rendering
        glfwPollEvents();                   // check for keyboard, mouse inputs etc.
    }

    glfwTerminate();
    return 0;
}