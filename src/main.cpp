#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>

const char* vertexShaderSource = "#version 330 core\n"
                           "layout (location = 0) in vec3 aPos;\n"
                           "layout (location = 1) in vec3 aColor;\n"
                           "out vec3 ourColor;\n"
                           "void main() {\n"
                           "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                           "ourColor = aColor;\n"
                           "}\0";

const char* aFragmentShaderSource = "#version 330 core\n"
                           "out vec4 fragColor;\n"
                           "in vec3 ourColor;\n"
                           "void main() {\n"
                           "fragColor = vec4(ourColor, 1.0f);\n"
                           "}\0";

const char* bFragmentShaderSource = "#version 330 core\n"
                           "out vec4 fragColor;\n"
                           "void main() {\n"
                           "fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                           "}\0";

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

    // ------------------------------------------------------------------------------------------------ glfw setup

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

    // ------------------------------------------------------------------------------------------------ glad setup

    // GLAD is used for locating OpenGL function pointers, so before we start using OpenGL directly we initialise it here
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // ------------------------------------------------------------------------------------------------ shader setup

    // compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // we will check if our shader compiled successfully :)
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // compile 1st fragment shader
    unsigned int aFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(aFragmentShader, 1, &aFragmentShaderSource, NULL);
    glCompileShader(aFragmentShader);

    // we will check if our shader compiled successfully :)
    glGetShaderiv(aFragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(aFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // compile 2nd fragment shader
    unsigned int bFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(bFragmentShader, 1, &bFragmentShaderSource, NULL);
    glCompileShader(bFragmentShader);

    // we will check if our shader compiled successfully :)
    glGetShaderiv(bFragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(bFragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader programme
    unsigned int aShaderProgram = glCreateProgram();
    glAttachShader(aShaderProgram, vertexShader);
    glAttachShader(aShaderProgram, aFragmentShader);
    glLinkProgram(aShaderProgram);

    // we will check for linking errors :)
    glGetShaderiv(aShaderProgram, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(aShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader programme
    unsigned int bShaderProgram = glCreateProgram();
    glAttachShader(bShaderProgram, vertexShader);
    glAttachShader(bShaderProgram, bFragmentShader);
    glLinkProgram(bShaderProgram);

    // we will check for linking errors :)
    glGetShaderiv(bShaderProgram, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(bShaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // we dont need these anymore :) <3
    glDeleteShader(vertexShader);
    glDeleteShader(aFragmentShader);
    glDeleteShader(bFragmentShader);

    // ------------------------------------------------------------------------------------------------ vertex data & buffers

    // tell OpenGL our window width and height
    glViewport(0, 0, 800, 600);

    float aVertices[] = {
    //   x      y      z  :  r     g     b
         0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f
    };

    float bVertices[] = {
    //   x      y      z  :  r     g     b
         0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
         1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
         0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 1.0f
    };

    // unsigned int aVBO, aVAO, bVBO, bVAO;
    unsigned int VBOs[2], VAOs[2];

    glGenVertexArrays(2, VAOs); // generate our VAO

    glGenBuffers(2, VBOs);      // generate our VBO

    glBindVertexArray(VAOs[0]);     // bind our VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aVertices), aVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind our VBO
    glBindVertexArray(0); // unbind our VAO

    glBindVertexArray(VAOs[1]);     // bind our VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bVertices), bVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind our VBO
    glBindVertexArray(0); // unbind our VAO

    // see framebuffer_size_callback()
    // we just need to tell GLFW we want it to use this function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    while(!glfwWindowShouldClose(window)) { // self-explanatory - this is our render loop :)
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state setting function
        glClear(GL_COLOR_BUFFER_BIT);         // state using function

        glUseProgram(aShaderProgram);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glUseProgram(bShaderProgram);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);            // double buffered rendering
        glfwPollEvents();                   // check for keyboard, mouse inputs etc.
    }

    glfwTerminate();
    return 0;
}