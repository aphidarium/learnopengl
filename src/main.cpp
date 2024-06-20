#include <math.h>
#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "shader.h"
#include "model.h"

#define CAMERA_UP         glm::vec3(0.0f, 1.0f, 0.0f)
#define MOUSE_SENSITIVITY 0.1f

#define MAX_FOV     104.0f
#define MIN_FOV     1.0f
#define DEFAULT_FOV 60.0f

struct {
    glm::vec3 pos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    float pitch     = 0.0f;
    float yaw       = -90.0f;

    float speed = 0.025f;
    bool fast = false;
    bool slow = false;

    float fov       = DEFAULT_FOV;
} camera;

bool _w = false, _a = false, _s = false, _d = false;
bool flashlight = false;

// we should also define a callback function for if/when the user changes the width/height of the window
// GLFW can do this for us
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_W: _w = true; break;
            case GLFW_KEY_S: _s = true; break;
            case GLFW_KEY_A: _a = true; break;
            case GLFW_KEY_D: _d = true; break;

            case GLFW_KEY_F:
                flashlight = !flashlight;
                if (flashlight) {
                  std::cout << "Flashlight on" << std::endl;
                } else {
                  std::cout << "Flashlight off" << std::endl;
                }
                break;

            case GLFW_KEY_LEFT_SHIFT:
                camera.slow = false;
                camera.fast = true;
                std::cout << camera.fast << std::endl;
                break;

            case GLFW_KEY_LEFT_CONTROL:
                camera.slow = true;
                camera.fast = false;
                break;

            case GLFW_KEY_ESCAPE:
                if      (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W: _w = false; break;
            case GLFW_KEY_S: _s = false; break;
            case GLFW_KEY_A: _a = false; break;
            case GLFW_KEY_D: _d = false; break;

            case GLFW_KEY_LEFT_SHIFT:   camera.fast = false;  break;
            case GLFW_KEY_LEFT_CONTROL: camera.slow = false; break;
        }
    }
}

float lastX = 0, lastY = 0;
void mouse_callback(GLFWwindow* window, double x, double y) {
    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) return;

    xOffset *= MOUSE_SENSITIVITY;
    yOffset *= MOUSE_SENSITIVITY;

    camera.yaw   += xOffset;
    camera.pitch += yOffset;

    if (camera.pitch > 89.0f) {
        camera.pitch = 89.0f;
    }
    else if (camera.pitch < -89.0f) {
        camera.pitch = -89.0f;
    }

    glm::vec3 dir;
    dir.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    dir.y = sin(glm::radians(camera.pitch));
    dir.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = dir;
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.fov -= (float)yOffset;
    if      (camera.fov < MIN_FOV) camera.fov = MIN_FOV;
    else if (camera.fov > MAX_FOV) camera.fov = MAX_FOV;
}

void playerMovement() {
    float speed = camera.speed;
    if (camera.fast) {
      speed *= 2;
    } else if (camera.slow) {
      speed /= 2;
    }

    if (_w) camera.pos += camera.front * speed;
    if (_s) camera.pos -= camera.front * speed;
    if (_a) camera.pos -= glm::normalize(glm::cross(camera.front, CAMERA_UP)) * speed;
    if (_d) camera.pos += glm::normalize(glm::cross(camera.front, CAMERA_UP)) * speed;
}

int main() {
    glfwInit(); // initialise GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // configure with `glfwWindowHint`
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "floating", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to initialise window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // ------------------------------------------------------------------------------------------------ vertex data & buffers

    Model backpack = Model("backpack.obj");

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 lightPositions[] = {
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3( 0.0f,  1.0f,  0.0f),
        glm::vec3( 0.0f, -1.0f,  0.0f)
    };

    glm::vec3 lightColors[] = {
        glm::vec3(0.732f, 0.214f, 0.348f), // Burgundy
        glm::vec3(0.298f, 0.588f, 0.824f), // Light Blue
        glm::vec3(0.988f, 0.733f, 0.098f), // Gold
        glm::vec3(0.0f, 0.769f, 0.0f),     // Green
        glm::vec3(0.855f, 0.439f, 0.878f), // Light Pink
        glm::vec3(0.545f, 0.271f, 0.075f), // Brown
        glm::vec3(0.937f, 0.910f, 0.863f), // Light Yellow
        glm::vec3(0.388f, 0.294f, 0.329f), // Dark Plum
        glm::vec3(0.0f, 0.0f, 0.0f),       // Black
        glm::vec3(1.0f, 1.0f, 1.0f),       // White
    };

    Shader shader = Shader("src/shaders/default.vert", "src/shaders/default.frag");

    Shader phongLitShader   = Shader("src/shaders/default.vert", "src/shaders/phong/litobject.frag");
    Shader phongLightShader = Shader("src/shaders/default.vert", "src/shaders/phong/light.frag");

    Shader litShader   = phongLitShader;
    Shader lightShader = phongLightShader;

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.pos,
                                     camera.pos + camera.front,
                                     CAMERA_UP);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)(16/9), 0.1f, 100.0f);

        glm::vec3 lightColor  = glm::vec3(1);
        glm::vec3 lightPos = glm::vec3(3.6f, 0.0f, -3.6f);
        glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
        lightPos *= glm::vec3(sin(glfwGetTime()), 1.0f, cos(glfwGetTime()));

        litShader.use();
        litShader.setMat4("view",       view);
        litShader.setMat4("model",      model);
        litShader.setMat4("projection", projection);
        litShader.setVec3("viewPos",    camera.pos);

        litShader.setInt("material.diffuse",  0);
        litShader.setInt("material.specular", 0);
        litShader.setInt("material.emission", 1);

        litShader.setInt("usingFlashlight",   flashlight);
        if (flashlight) {
            litShader.setVec3("light.position",   camera.pos);
            litShader.setVec3("light.direction",  camera.front);
            litShader.setFloat("light.innerCone", cos(glm::radians(5.0f)));
            litShader.setFloat("light.outerCone", cos(glm::radians(35.0f)));
            litShader.setVec3("light.ambient",    glm::vec3(0.0));
            litShader.setVec3("light.diffuse",    lightColor);
            litShader.setVec3("light.specular",   glm::vec3(1.0));
            litShader.setFloat("light.constant",  1.0f);
            litShader.setFloat("light.linear",    0.05f);
            litShader.setFloat("light.quadratic", 0.032f);
        }

        litShader.setVec3("dirLight.direction", glm::vec3(-0.1f, -0.5f, -0.3f));
        litShader.setVec3("dirLight.ambient",   glm::vec3(0.0));
        litShader.setVec3("dirLight.diffuse",   glm::vec3(0.5, 0.6, 0.3));
        litShader.setVec3("dirLight.specular",  glm::vec3(1.0));

        for (int i = 0; i < 10; i++) {
          litShader.setVec3("pointLights[" + std::to_string(i) + "].position", lightPositions[i]);
          litShader.setVec3("pointLights[" + std::to_string(i) + "].ambient",  glm::vec3(0));
          litShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse",  lightColors[i] * glm::vec3(0.2f));
          litShader.setVec3("pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0));
          litShader.setFloat("pointLights[" + std::to_string(i) + "].constant",  1.0f);
          litShader.setFloat("pointLights[" + std::to_string(i) + "].linear",    0.05f);
          litShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }

        for (int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);

            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0f));
            model = glm::translate(model, cubePositions[i]);
            litShader.setFloat("material.shininess", pow(2, i+2));

            litShader.setMat4("model", model);
            backpack.draw(litShader);
        }

//        model = glm::mat4(1.0f);
//        model = glm::translate(model, lightPos);
//        model = glm::rotate(model, (float)(glfwGetTime()*0.75), glm::vec3(-1.0f, -0.3f, 0.2f));
//
//        lightShader.use();
//        lightShader.setMat4("view", view);
//        lightShader.setMat4("model", model);
//        lightShader.setMat4("projection", projection);
//        lightShader.setVec3("lightColor", lightColor);
//
        glfwSwapBuffers(window);            // double buffered rendering
        glfwPollEvents();                   // check for keyboard, mouse inputs etc.
        playerMovement();
    }

    glfwTerminate();
    return 0;
}
