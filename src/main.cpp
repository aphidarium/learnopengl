#include <math.h>
#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.h"

#define CAMERA_SPEED      0.05f
#define CAMERA_UP         glm::vec3(0.0f, 1.0f, 0.0f)
#define MOUSE_SENSITIVITY 0.5f

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront    = glm::vec3(0.0f, 0.0f, -1.0f);

// we should also define a callback function for if/when the user changes the width/height of the window
// GLFW can do this for us
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_W))
        cameraPosition += cameraFront * CAMERA_SPEED;
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_S))
        cameraPosition -= cameraFront * CAMERA_SPEED;
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_A))
        cameraPosition -= glm::normalize(glm::cross(cameraFront, CAMERA_UP)) * CAMERA_SPEED;
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_D))
        cameraPosition += glm::normalize(glm::cross(cameraFront, CAMERA_UP)) * CAMERA_SPEED;
    else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
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

    // ------------------------------------------------------------------------------------------------ vertex data & buffers

    // tell OpenGL our window width and height
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    float verts[] = { // cube
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO); // generate our VAO
    glGenBuffers(1, &VBO);      // generate our VBO

    glBindVertexArray(VAO);     // bind our VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    //                    location, size, type,     normalised, stride,            pointer
    glVertexAttribPointer(0,        3,    GL_FLOAT, GL_FALSE,   5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // for sending our colour to the vertex shader
    glVertexAttribPointer(2,        2,    GL_FLOAT, GL_FALSE,   5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind our VBO
    glBindVertexArray(0); // unbind our VAO

    // stbi_set_flip_vertically_on_load(true);
    auto loadImage = [](std::string file, int format, uint texture){
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(file.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load image '" << file << "'" << std::endl;
        }

        stbi_image_free(data);
    };

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    loadImage("/home/alfr/projects/code/cpp/opengl/awesomeface.png", GL_RGBA, texture1);
    loadImage("/home/alfr/projects/code/cpp/opengl/alfie.jpg", GL_RGB, texture2);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader shader = Shader("/home/alfr/projects/code/cpp/opengl/src/shaders/default.vs", "/home/alfr/projects/code/cpp/opengl/src/shaders/default.fs");

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    while(!glfwWindowShouldClose(window)) { // self-explanatory - this is our render loop :)
        processInput(window);

        glClearColor(0.4f, 0.6f, 0.84f, 1.0f); // state setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // state using function

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::lookAt(cameraPosition,  // position
                                     cameraPosition + cameraFront,  // target
                                     CAMERA_UP); // up

        int w = 0, h = 0;
        glfwGetWindowSize(window, &w, &h);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)(w/h), 0.1f, 100.0f);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));

        shader.use();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"),       1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform"),  1, GL_FALSE, glm::value_ptr(transform));
        shader.setFloat("mixAmount", sin(glfwGetTime()));
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);            // double buffered rendering
        glfwPollEvents();                   // check for keyboard, mouse inputs etc.
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}