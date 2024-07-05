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
#include "sprite.h"
#include "entity/prop.h"
#include "entity/light/directionalLight.h"
#include "entity/light/pointLight.h"
#include "entity/light/spotLight.h"

#define CAMERA_UP         glm::vec3(0.0f, 1.0f, 0.0f)
#define MOUSE_SENSITIVITY 0.1f

#define MAX_FOV     104.0f
#define MIN_FOV     1.0f
#define DEFAULT_FOV 60.0f

#define TWO_PI 6.28319

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

            case GLFW_KEY_F: flashlight = !flashlight; break;

            case GLFW_KEY_LEFT_SHIFT:
                camera.slow = false;
                camera.fast = true;
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

    if (camera.pitch > 89.9f) {
        camera.pitch = 89.9f;
    }
    else if (camera.pitch < -89.9f) {
        camera.pitch = -89.9f;
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

    Prop asteroid1 = Prop(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), "assets/asteroid1.obj");
    Prop asteroid2 = Prop(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), "assets/asteroid2.obj");
    Prop asteroid3 = Prop(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), "assets/asteroid3.obj");

    //Model backpack = Model("assets/asteroid.obj");

    Sprite sprite = Sprite("assets/lightbulb.png", glm::vec3(1.0, 1.0, 0.0));

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

    Shader litShader   = Shader("src/shaders/default.vert", "src/shaders/phong/litobject.frag");
    //Shader lightShader = Shader("src/shaders/default.vert", "src/shaders/phong/light.frag");
    //Shader spriteShader = Shader("src/shaders/sprite/sprite.vert", "src/shaders/sprite/sprite.frag");

    DirectionalLight dirLight = DirectionalLight(glm::vec3(-0.1f, -0.5f, -0.3f), glm::vec3(0.0f), glm::vec3(0.98f, 0.95f, 0.84f), glm::vec3(1.0f));
    dirLight.setDirection(glm::vec3(0.0f, -1.0f, 0.0f));

    std::vector<PointLight> pointLights;
    for (uint i = 0; i < 0; i++) {
      PointLight p = PointLight(lightPositions[i], glm::vec3(0.0f), lightColors[i] * glm::vec3(0.2f), glm::vec3(1.0f));
      pointLights.push_back(p);
    }

    std::vector<SpotLight> spotLights;
    SpotLight flashlightLight = SpotLight(camera.pos, camera.front, 5.0f, 35.0f, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(camera.pos, camera.pos + camera.front, CAMERA_UP);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)(16/9), 0.1f, 100.0f);

        litShader.use();
        litShader.setMat4("view",       view);
        litShader.setMat4("model",      model);
        litShader.setMat4("projection", projection);
        litShader.setVec3("viewPos",    camera.pos);

        dirLight.addToShader(litShader);

        litShader.setInt("spotLightAmount", spotLights.size());
        for (uint i = 0; i < spotLights.size(); i++)
          spotLights[i].addToShader(litShader);

        litShader.setInt("usingFlashlight", flashlight);
        flashlightLight.setPosition(camera.pos);
        flashlightLight.setDirection(camera.front);
        if (flashlight) flashlightLight.addToShader(litShader, "flashlight");

        litShader.setInt("pointLightAmount", (int)pointLights.size());
        for (uint i = 0; i < pointLights.size(); i++)
          pointLights[i].addToShader(litShader, i);

        litShader.setFloat("material.shininess", 32);

        // ---------- ASTEROIDS

        asteroid1.setRotationX(glfwGetTime() / 100);
        asteroid1.setRotationY(glfwGetTime() / 64);
        asteroid1.setPosition(sin(glfwGetTime() / 190) * 24, 0.0f, cos(glfwGetTime() / 174) * 20);
        asteroid1.draw(litShader);

        asteroid2.setRotationX(glfwGetTime() / 92);
        asteroid2.setRotationY(glfwGetTime() / 54);
        asteroid2.setRotationZ(sin(glfwGetTime()/64) / 2);
        asteroid2.setPosition(sin(glfwGetTime() / 95) * 3.4f, sin(glfwGetTime() / 75) * 3.4f, -5.0f);
        asteroid2.draw(litShader);

        asteroid3.setRotationX(glfwGetTime() / 100);
        asteroid3.setRotationY(glfwGetTime() / 64);
        asteroid3.setPosition(-sin(glfwGetTime() / 140) * 18, 0.0f, -cos(glfwGetTime() / 134) * 12);
        asteroid3.draw(litShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        playerMovement();
    }

    glfwTerminate();
    return 0;
}
