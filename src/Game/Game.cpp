#include "Game.hpp"

#include "../Lib.hpp"
#include "../Color/Color.hpp"
#include "../Camera/Camera.hpp"
#include "../Window/Window.hpp"
#include "../Shape/Shape3D.hpp"
#include "../Shape/Cubemap.hpp"
#include "../Shape/Cube.hpp"
#include "../Scene/Scene.hpp"
#include "../Chunk/Chunk.hpp"

Game::Game(unsigned int width, unsigned int height)
{
    this->state = GAME_NONE;
    this->width = width;
    this->height = height;
#define WIDTH width
#define HEIGHT height

}

mat4 projection;
Scene scene;
Camera camera = Camera();
Cubemap* skybox = new Cubemap();
Shader cubeShader, skyboxShader;

struct Mouse
{
    bool firstMouse = true;
    float lastX;
    float lastY;
} mouse;

void mouseMovementCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    
    if (mouse.firstMouse)
    {
        mouse.firstMouse = false;
        mouse.lastX = xpos;
        mouse.lastY = ypos;
    }

    float xoffset = xpos - mouse.lastX;
    float yoffset = ypos - mouse.lastY;

    mouse.lastX = xpos;
    mouse.lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void Game::init(Window* window)
{
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
    scene = Scene(projection);

    cubeShader = Shader("./resources/shaders/vertexShader.glsl", "./resources/shaders/fragmentShader.glsl");
    cubeShader.use();
    cubeShader.setMat4("projection", projection);

    skyboxShader = Shader("./resources/shaders/vertexShaderSkybox.glsl", "./resources/shaders/fragmentShaderSkybox.glsl");
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setInt("skybox", 0);

    skybox->createVertexArray();

    camera.moveCamera(vec3(0, 0, 2));

    // sets the mouse callback function
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window->getWindow(), mouseMovementCallback);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window->getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    this->state = GAME_ACTIVE;
}

void Game::processInput(float deltaTime, Window window)
{
    // camera input
    float cameraVelocity = camera.getCameraVelocity() * deltaTime;
    if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_SHIFT)  == GLFW_PRESS)
    {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
}

void Game::update(float deltaTime)
{
    cubeShader.use();
    cubeShader.setMat4("view", camera.getViewMatrix());
    // sending variables to skybox shader
    skyboxShader.use();
    skyboxShader.setMat4("view", mat4(mat3(camera.getViewMatrix())));
}

void Game::render()
{
    // draw the game scene

    // draw the skybox
    skyboxShader.use();
    skybox->draw(skyboxShader);
    cubeShader.use();
    Chunk chunk = Chunk(0);
    chunk.generateChunk();
    chunk.drawChunk(cubeShader);
}

void Game::clear()
{
    scene.clear();
}

