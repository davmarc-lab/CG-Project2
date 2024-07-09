#include "PlayState.hpp"

#include "../Camera/Camera.hpp"
#include "../Color/Color.hpp"
#include "../Entity/Collider/SphereCollider.hpp"
#include "../Entity/Cube.hpp"
#include "../Entity/Cubemap.hpp"
#include "../Entity/Plane.hpp"
#include "../Entity/Sphere.hpp"

#include "../Scene/Scene.hpp"

#include "../Menu/IGEntity.hpp"
#include "../Menu/IGMenu.hpp"
#include "../Menu/IGMode.hpp"
#include "../Menu/IGCamera.hpp"

PlayState PlayState::playState;

CubeEntity *cube;
Sphere *sphere;
PlaneEntity *plane;
Cubemap *skybox;
Shader cubeShader, planeShader, skyboxShader;

Scene s;

InputMode user_mode = InputMode::INTERACT;

IGMenu *entityMenu, *modeMenu, *cameraMenu;

static Mouse mouse;

void PlayState::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    cubeShader = Shader("./resources/shaders/vertexShader.glsl", "./resources/shaders/fragmentShader.glsl");
    cubeShader.use();
    cubeShader.setMat4("projection", projection);

    Texture texture = Texture("./resources/textures/web-dirt.png");
    texture.createTexture();

    cube = new CubeEntity();
    cube->createVertexArray();
    cube->setScale(vec3(1));
    cube->applyTransformation(vec3(0), vec3(1), vec3(0), 0);
    cube->attachTexture(texture);
    cube->setCollider(new CubeCollider(cube->getTransform()));
    cube->setDynamic();

    planeShader = Shader("./resources/shaders/vertexShader.glsl", "./resources/shaders/fragmentShader.glsl");
    planeShader.use();
    planeShader.setMat4("projection", projection);
    planeShader.setInt("color", 1);

    plane = new PlaneEntity(color::RED);
    plane->createVertexArray();

    skyboxShader = Shader("./resources/shaders/vertexShaderSkybox.glsl", "./resources/shaders/fragmentShaderSkybox.glsl");
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setInt("skybox", 0);

    skybox = new Cubemap();
    skybox->createVertexArray();

    // add elements to the scene
    s.addElement(plane, planeShader);
    s.addElement(cube, cubeShader);

    // imgui
    entityMenu = new IGEntity(cube);
    modeMenu = new IGMode(&user_mode);
    cameraMenu = new IGCamera();

    camera.moveCamera(vec3(0, 0, 3));
}

void PlayState::clean() {}

void PlayState::pause() {}

void PlayState::resume() {}

GLenum val = GL_FALSE;

void clearMouseFunc(GLFWwindow *window, double x, double y) { ImGui_ImplGlfw_CursorPosCallback(window, x, y); }

void passiveMouseFunc(GLFWwindow *window, double x, double y) {
    if (mouse.first_mouse) {
        mouse.first_mouse = false;
        mouse.lastX = x;
        mouse.lastY = y;
    }

    auto xoffset = x - mouse.lastX;
    auto yoffset = y - mouse.lastY;

    mouse.lastX = x;
    mouse.lastY = y;

    camera.processMouseMovement(xoffset, yoffset);

    // ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

void PlayState::handleEvent(GameEngine *engine) {
    auto window = engine->getWindow()->getWindow();
    const float dt = engine->getDeltaTime();

    // Change mode
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        user_mode = InputMode::INTERACT;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        user_mode = InputMode::SELECT;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        user_mode = InputMode::PASSIVE;
    }

    switch (user_mode) {
    case SELECT:
        // enable selecting object mode
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, clearMouseFunc);
        break;
    case INTERACT:
        // enable movement in the scene, and active mouse movement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, clearMouseFunc);
        break;
    case PASSIVE:
        // enable passive mouse movement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, passiveMouseFunc);

        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        break;
    default:
        break;
    }

    // Mouse input
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    y = HEIGHT - y;
    vec2 mouse_pos = vec2(x, y);

    // Camera input
    float cameraVelocity = camera.getCameraVelocity() * engine->getDeltaTime();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // lockKey(GLFW_KEY_W);
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        // cube jump
        // PhysicWorld::jump(cube, dt);
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
}

void updatePosition(Entity *ent) {
    // reset model matrix
    ent->setModelMatrix(ent->refreshModelMatrix());
}

void PlayState::update(GameEngine *engine) {
    cubeShader.use();
    cubeShader.setMat4("view", camera.getViewMatrix());

    planeShader.use();
    planeShader.setMat4("view", camera.getViewMatrix());

    skyboxShader.use();
    skyboxShader.setMat4("view", mat4(mat3(camera.getViewMatrix())));

    updatePosition(cube);
}

void PlayState::draw(GameEngine *engine) {

    skybox->draw(skyboxShader);

    s.draw();

    entityMenu->render();
    modeMenu->render();
    cameraMenu->render();
}
