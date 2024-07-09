#include "PlayState.hpp"

#include "../Camera/Camera.hpp"
#include "../Color/Color.hpp"
#include "../Entity/Collider/SphereCollider.hpp"
#include "../Entity/Cube.hpp"
#include "../Entity/Cubemap.hpp"
#include "../Entity/Plane.hpp"
#include "../Entity/Sphere.hpp"

#include "../Scene/Scene.hpp"

#include "../Menu/IGCamera.hpp"
#include "../Menu/IGEntity.hpp"
#include "../Menu/IGMenu.hpp"
#include "../Menu/IGMode.hpp"
#include <algorithm>

PlayState PlayState::playState;

CubeEntity *cube;
Sphere *sphere;
PlaneEntity *plane;
Cubemap *skybox;
Shader cubeShader, planeShader, skyboxShader;

Scene obj_scene;

InputMode user_mode = InputMode::INTERACT;

IGMenu *modeMenu, *cameraMenu;
IGEntity *entityMenu = nullptr;

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

    sphere = new Sphere();
    sphere->createVertexArray();
    sphere->setScale(vec3(1));
    sphere->applyTransformation(vec3(0), vec3(1), vec3(0), 0);

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
    obj_scene.addElement(cube, cubeShader);
    obj_scene.addElement(sphere, cubeShader);

    // imgui
    entityMenu = new IGEntity();
    modeMenu = new IGMode(&user_mode);
    cameraMenu = new IGCamera();

    camera.moveCamera(vec3(0, 0, 3));
}

void PlayState::clean() {}

void PlayState::pause() {}

void PlayState::resume() {}

GLenum val = GL_FALSE;

void clearCursorPosFunc(GLFWwindow *window, double x, double y) { ImGui_ImplGlfw_CursorPosCallback(window, x, y); }

void passiveCursorPosFunc(GLFWwindow *window, double x, double y) {
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

vec3 getRayFromMouse(int mouse_x, int mouse_y) {
    mouse_y = HEIGHT - mouse_y;

    float ndc_x = (2.0f * mouse_x) / WIDTH - 1.0f;
    float ndc_y = (2.0f * mouse_y) / HEIGHT - 1.0f;
    float ndc_z = -1.0f;

    vec4 clip = vec4(ndc_x, ndc_y, ndc_z, 1.0f);

    vec4 view_model_p = inverse(projection) * clip;
    view_model_p.w = 1;

    vec4 pw = inverse(camera.getViewMatrix()) * view_model_p;

    return normalize(vec3(pw) - vec3(camera.getCameraPosition()));
}

bool isRayInSphere(vec3 ray, vec3 sphere_pos, float sphere_radius, float *id) {
    vec3 d = camera.getCameraPosition() - sphere_pos;
    float b = dot(d, ray);
    float cc = dot(d, d) - sphere_radius * sphere_radius;
    float delta = b * b - cc;

    if (delta < 0.0f)
        return false;
    else if (delta > 0.0f) {
        float t_a = -b + sqrt(delta);
        float t_b = -b - sqrt(delta);
        *id = t_b;

        if (t_a < 0.0f && t_b < 0.0f)
            return false;

        return true;
    } else { // delta == 0.0f
        float t = -b + sqrt(delta);
        if (t < 0)
            return false;
        *id = t;
        return true;
    }
}

void selectMouseFunc(GLFWwindow *window, int button, int action, int mod) {
    // select objects
    if (button == GLFW_MOUSE_BUTTON_1) {
        if (action == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            float xmouse = x;
            float ymouse = y;

            vec3 ray = getRayFromMouse(xmouse, ymouse);
            // closest_intersection
            float ci = 0;
            // this pointer keeps track of the selected object
            Entity* selected = nullptr;
            
            // check all the element of the scene for intersection with ray
            for(auto elem : obj_scene.getElements()) {
                auto obj = elem.first;
                // distance between ray and object
                float dist = 0.f;

                if (isRayInSphere(ray, obj->getPosition(), 0.7, &dist)) {
                    if (selected == nullptr || dist <= ci) {
                        selected = obj;
                        ci = dist;
                    }
                }
            }

            // updating the observer in IGEntity menu
            if (selected != nullptr) {
                entityMenu->changeObserver(selected);
            } else {
                entityMenu->changeObserver(nullptr);
            }
        }
    }

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mod);
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
        glfwSetCursorPosCallback(window, clearCursorPosFunc);

        glfwSetMouseButtonCallback(window, selectMouseFunc);
        break;
    case INTERACT:
        // enable movement in the scene, and active mouse movement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(window, clearCursorPosFunc);
        break;
    case PASSIVE:
        // enable passive mouse movement
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, passiveCursorPosFunc);

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
    updatePosition(sphere);
}

void PlayState::draw(GameEngine *engine) {

    skybox->draw(skyboxShader);
    plane->draw(planeShader);

    obj_scene.draw();

    entityMenu->render();
    modeMenu->render();
    cameraMenu->render();
}
