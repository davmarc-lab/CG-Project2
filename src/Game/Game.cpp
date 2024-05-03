#include "Game.hpp"

#include "../Camera/Camera.hpp"
#include "../Chunk/Chunk.hpp"
#include "../Scene/Scene.hpp"
#include "../Shape/Cubemap.hpp"
#include "../Shape/Model.hpp"
#include "../Window/Crosshair.hpp"
#include "../Window/Window.hpp"

Game::Game(unsigned int width, unsigned int height) {
    this->state = GAME_NONE;
    this->width = width;
    this->height = height;
#define WIDTH width
#define HEIGHT height
}

Scene scene;
Camera camera = Camera();
Cubemap *skybox = new Cubemap();
Cube *cube, *tmp;
Model *bean;
Shader cubeShader, modelShader, skyboxShader, outlineShader;
Crosshair *c;

struct Mouse {
    bool firstMouse = true;
    float lastX;
    float lastY;
} mouse;

void mouseMovementCallback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (mouse.firstMouse) {
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

void Game::init(Window *window) {
    scene = Scene(projection);

    camera.moveCamera(vec3(0, 0, 0));

    cubeShader = Shader("./resources/shaders/vertexShader.glsl",
                        "./resources/shaders/fragmentShader.glsl");
    cubeShader.use();
    cubeShader.setMat4("projection", projection);

    Texture texture =
        Texture("resources/textures/web-dirt.png", Cube().getTextureCoords());
    texture.createTexture();

    cube = new Cube(color::RED);
    cube->createVertexArray();
    cube->attachTexture(texture);
    cube->transformMesh(vec3(0), vec3(BLOCK_DIM), vec3(0), 0);

    printVec3(camera.getCameraPosition() * vec3(1));

    tmp = new Cube(color::RED);
    tmp->createVertexArray();
    tmp->attachTexture(texture);
    tmp->transformMesh(vec3(0), vec3(BLOCK_DIM + OUTLINE_DIM), vec3(0), 0);

    modelShader = Shader("./resources/shaders/modelVertexShader.glsl",
                         "./resources/shaders/modelFragmentShader.glsl");
    modelShader.use();
    modelShader.setMat4("projection", projection);

    skyboxShader = Shader("./resources/shaders/vertexShaderSkybox.glsl",
                          "./resources/shaders/fragmentShaderSkybox.glsl");
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setInt("skybox", 0);

    outlineShader = Shader("./resources/shaders/vertexShader.glsl",
                           "./resources/shaders/outlineShader.glsl");
    outlineShader.use();
    outlineShader.setMat4("projection", projection);

    skybox->createVertexArray();

    // Troll
    // bean = new Model("./resources/models/bean/bean.obj");

    // bag = new Model("./resources/models/backpack/backpack.obj",
    // Flip::VERTICALLY);

    // sets the mouse callback function
    glfwSetInputMode(window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window->getWindow(), mouseMovementCallback);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window->getWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    GLFWcursor *cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    glfwSetCursor(window->getWindow(), cursor);

    c = new Crosshair();
    c->initializeCrosshair();

    this->state = GAME_ACTIVE;
}

void Game::processInput(float deltaTime, Window window) {
    // camera input
    float cameraVelocity = camera.getCameraVelocity() * deltaTime;
    if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() +
                   cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() -
                   cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() -
                   cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() +
                   cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
        auto pos =
            camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window.getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        auto pos =
            camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
}

void checkCameraCollision() {
    for (const auto elem: scene.getSceneElements()) {
    }
}

void Game::update(float deltaTime) {
    cubeShader.use();
    cubeShader.setMat4("view", camera.getViewMatrix());

    outlineShader.use();
    outlineShader.setMat4("view", camera.getViewMatrix());

/*     modelShader.use();
    modelShader.setMat4("view", camera.getViewMatrix());
    auto model = mat4(1.f);
    model = translate(model, vec3(0, 0, 2));
    model = scale(model, vec3(0.1));
    modelShader.setMat4("model", model); */

    // sending variables to skybox shader
    skyboxShader.use();
    skyboxShader.setMat4("view", mat4(mat3(camera.getViewMatrix())));

    checkCameraCollision();
    cout << "CAMERA POS" << endl;
    printVec3(camera.getCameraPosition() + vec3(0.1));
    cout << cube->isColliding(camera.getCameraPosition() - vec3(0.1), camera.getCameraPosition() + vec3(0.1)) << endl;
}

void Game::render() {
    // draw the game scene

    // draw the skybox
    skyboxShader.use();
    skybox->draw(skyboxShader);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    cubeShader.use();
    cube->draw(cubeShader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    outlineShader.use();
    tmp->draw(outlineShader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

    /* modelShader.use();
    bean->draw(modelShader); */

    /*   modelShader.use();
        bag->draw(modelShader); */
    c->drawCrosshair(camera.getViewMatrix());
    // use border shader and draw again
}

void Game::clear() { scene.clear(); }
