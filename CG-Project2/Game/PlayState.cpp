#include "PlayState.hpp"

#include "../Camera/Camera.hpp"
#include "../Color/Color.hpp"
#include "../Entity/Cube.hpp"
#include "../Entity/Cubemap.hpp"
#include "../Entity/Object.hpp"
#include "../Entity/Plane.hpp"
#include "../Entity/Sphere.hpp"

#include "../Scene/Scene.hpp"

#include "../Menu/IGCamera.hpp"
#include "../Menu/IGEntity.hpp"
#include "../Menu/IGLights.hpp"
#include "../Menu/IGMenu.hpp"
#include "../Menu/IGMode.hpp"
#include "../Menu/IGMousePopup.hpp"

#include "Game.hpp"

PlayState PlayState::playState;
string mouse_popup_name = "Menu";

CubeEntity *cube;
Sphere *sphere;
PlaneEntity *plane;
Cubemap *skybox;
Shader lightShader, planeShader, skyboxShader, modelShader;

PointLight *pl = new PointLight();
SpotLight *ll = new SpotLight();

Object *obj;

Entity *obj_selected = nullptr;
Shader *shader_selected = nullptr;

Scene obj_scene;

InputMode user_mode = InputMode::INTERACT;

IGMenu *modeMenu, *cameraMenu, *lightsMenu;
IGEntity *entityMenu = nullptr;
IGMousePopup *mousePopup = new IGMousePopup(mouse_popup_name.c_str());

bool show_popup = false;
bool show_object_picker = false;

Mouse mouse;

ActionManager *action_manager = ActionManager::instance();

void PlayState::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    lightShader = Shader("./resources/shaders/lightVertexShader.glsl", "./resources/shaders/lightFragmentShader.glsl");
    Texture cube_texture = Texture("./resources/textures/web-dirt.png");
    cube_texture.createTexture();

    cube = new CubeEntity();
    cube->createVertexArray();
    cube->setPosition(vec3(0, 1, 0));
    cube->setScale(vec3(1));
    cube->attachTexture(cube_texture);
    // cube->setMaterial(material::NONE);

    Texture sphere_texture = Texture("./resources/textures/web-dirt.png");
    sphere_texture.createTexture();

    sphere = new Sphere();
    sphere->createVertexArray();
    sphere->setPosition(vec3(2, 0, 0));
    sphere->setScale(vec3(1));
    sphere->attachTexture(sphere_texture);
    // sphere->setMaterial(material::NONE);

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
    obj_scene.addElement(cube, &lightShader);
    obj_scene.addElement(sphere, &lightShader);

    pl->initCaster();
    pl->setPosition(vec3(0, -2, 0));
    ll->initCaster();
    ll->setPosition(vec3(1));
    ll->setDirection(vec3(0, 1, 0));

    obj_scene.addLight(pl);
    obj_scene.addLight(ll);

    modelShader = Shader("./resources/shaders/lightVertexShader.glsl", "./resources/shaders/lightFragmentShader.glsl");
    modelShader.use();
    modelShader.setMat4("projection", projection);

    obj = new Object("./resources/models/backpack/backpack.obj", Flip::VERTICALLY);
    obj->setPosition(vec3(0));
    obj->setScale(vec3(0.5));
    obj->setMaterial(material::NONE);
    obj_scene.addElement(obj, &lightShader);

    // imgui
    entityMenu = new IGEntity();
    modeMenu = new IGMode(&user_mode);
    cameraMenu = new IGCamera();
    lightsMenu = new IGLights(obj_scene.getLights());
}

void PlayState::clean() {}

void PlayState::pause() {}

void PlayState::resume() {}

GLenum val = GL_FALSE;

void clearCursorPosFunc(GLFWwindow *window, double x, double y) { ImGui_ImplGlfw_CursorPosCallback(window, x, y); }

void clearMouseActionFunc(GLFWwindow *window, int button, int action, int mod) { ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mod); }

vec3 getTrackBallPoint(float x, float y) {
    float zTemp;
    vec3 point;

    point.x = (2.0f * x - WIDTH) / WIDTH;
    point.y = (HEIGHT - 2.0f * y) / HEIGHT;

    zTemp = 1.0f - pow(point.x, 2.0) - pow(point.y, 2.0);
    if (zTemp < 0)
        point.z = 0;
    else
        point.z = sqrt(zTemp);
    return normalize(point);
}

void mouseActiveMotion(GLFWwindow *window, double x, double y) {
    float speed = camera.getTrackballSpeed();

    vec3 p2 = getTrackBallPoint(x, y);
    vec3 p1 = getTrackBallPoint(mouse.lastX, mouse.lastY);

    if (!mouse.active_mode) {
        mouse.lastX = x;
        mouse.lastY = y;
        ImGui_ImplGlfw_CursorPosCallback(window, x, y);
        return;
    }

    float dx, dy, dz;
    dx = p2.x - p1.x;
    dy = p2.y - p1.y;
    dz = p2.z - p1.z;

    if (dx || dy || dz) {
        float angle = acos(dot(p1, p2)) * speed;
        vec3 rotation_vec = glm::cross(p1, p2);
        camera.setCameraDirection(vec4(camera.getCameraPosition(), 0) - vec4(camera.getCameraTarget(), 0));
        camera.moveCamera(vec4(camera.getCameraTarget(), 0) +
                          rotate(glm::mat4(1.0f), glm::radians(-angle), rotation_vec) * vec4(camera.getCameraDirection(), 0));
    }
    mouse.lastX = x;
    mouse.lastY = y;

    ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

// This function detects if right click of the mouse is pressed and opens a popup
void imGuiMouse2Popup() {
    // If you open moultiple popup it broke
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup(mousePopup->getPopupStringId());
    }
}

void mouseInputFunc(GLFWwindow *window, int button, int action, int mod) {

    // for trackball input
    if (button == GLFW_MOUSE_BUTTON_3) {
        if (action == GLFW_PRESS) {
            mouse.active_mode = true;
        } else if (action == GLFW_RELEASE) {
            mouse.active_mode = false;
        }
    }

    // you can handle your ImGui::popup here
    // that's because you cannot see the mouse in passive mode
    // in select mode with left click you select objects

    // seg fault in IsPopupOpen :/
    /* if (user_mode == InputMode::INTERACT) {
        // handle mouse input using ImGui
        if (button == GLFW_MOUSE_BUTTON_2) {
            // open popup
            show_popup = true;
        }

        // left click out of the popup
        if (show_popup && button == GLFW_MOUSE_BUTTON_1 && ImGui::IsPopupOpen(popup_name.c_str())) {
            show_popup = false;
        }
    } */

    imGuiMouse2Popup();

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mod);
}

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

    // this should be useless, because the mouse is hidden when using passive mode
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
    // trackball movement
    if (button == GLFW_MOUSE_BUTTON_3) {
        if (action == GLFW_PRESS) {
            mouse.active_mode = true;
        } else if (action == GLFW_RELEASE) {
            mouse.active_mode = false;
        }
    }
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
            obj_selected = nullptr;
            shader_selected = nullptr;

            // check all the element of the scene for intersection with ray
            for (auto elem : obj_scene.getElements()) {
                auto current = elem.first;
                auto shader = elem.second;
                // distance between ray and object
                float dist = 0.f;

                if (isRayInSphere(ray, current->getPosition(), 0.7, &dist)) {
                    if (obj_selected == nullptr || dist <= ci) {
                        obj_selected = current;
                        warning("Working on the same shader, it affects all the shaders");
                        shader_selected = shader;
                        ci = dist;
                    }
                }
            }

            for (auto caster : obj_scene.getLights()) {
                float dist = 0.f;

                if (isRayInSphere(ray, caster->getCaster()->getPosition(), 0.7, &dist)) {
                    if (obj_selected == nullptr || dist <= ci) {
                        obj_selected = caster->getCaster();
                        warning("Working on the same shader, it affects all the shaders");
                        shader_selected = caster->getShader();
                        ci = dist;
                    }
                }
            }

            // updating the observer in IGEntity menu
            if (obj_selected != nullptr) {
                entityMenu->changeObserver(obj_selected, shader_selected);
            } else {
                entityMenu->resetObserver();
            }
        }
    }

    imGuiMouse2Popup();

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mod);
}

void PlayState::handleEvent(GameEngine *engine) {
    auto window = engine->getWindow()->getWindow();
    const float dt = engine->getDeltaTime();

    // Change mode
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_G)) {
        user_mode = InputMode::INTERACT;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_V)) {
        user_mode = InputMode::SELECT;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_P)) {
        user_mode = InputMode::PASSIVE;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_Q)) {
        this->clean();
        engine->quit();
    }

    switch (user_mode) {
    case SELECT:
        // enable selecting object mode
        mouse.first_mouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetMouseButtonCallback(window, selectMouseFunc);
        glfwSetCursorPosCallback(window, mouseActiveMotion);
        break;
    case INTERACT:
        // enable movement in the scene, and active mouse movement
        mouse.first_mouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetMouseButtonCallback(window, mouseInputFunc);
        glfwSetCursorPosCallback(window, mouseActiveMotion);
        break;
    case PASSIVE:
        // enable passive mouse movement
        mouse.first_mouse = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, passiveCursorPosFunc);
        glfwSetMouseButtonCallback(window, clearMouseActionFunc);

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
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
}

void PlayState::update(GameEngine *engine) {
    if (action_manager->isActionPresent()) {
        for (auto act : action_manager->getActions()) {
            switch (act) {
            case Action::ADD_CUBE_ENTITY:
                obj_scene.addElement(new CubeEntity(), &lightShader);
                break;
            case Action::ADD_SPHERE_ENTITY:
                obj_scene.addElement(new Sphere(), &lightShader);
                break;
            case Action::ADD_OBJECT_ENTITY: {
                show_object_picker = true;
                break;
            }
            case Action::DEL_ENTITY:
                if (obj_selected != nullptr) {
                    obj_scene.removeElement(obj_selected, shader_selected);
                    obj_selected = nullptr;
                    shader_selected = nullptr;
                    entityMenu->resetObserver();
                }
                break;
            default:
                break;
            }
        }
        action_manager->clear();
    }

    lightShader.use();
    lightShader.setMat4("view", camera.getViewMatrix());
    lightShader.setVec3("viewPos", camera.getCameraPosition());

    modelShader.use();
    modelShader.setMat4("view", camera.getViewMatrix());
    modelShader.setVec3("viewPos", camera.getCameraPosition());

    planeShader.use();
    planeShader.setMat4("view", camera.getViewMatrix());

    skyboxShader.use();
    skyboxShader.setMat4("view", mat4(mat3(camera.getViewMatrix())));

    // send custom view matrix to lights
    pl->setCustomView(camera.getViewMatrix());
    ll->setCustomView(camera.getViewMatrix());
}

void showObjectPicker() {
    IGFD::FileDialogConfig config;
    config.path = "./resources/models/";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseObject", "Choose Object File", ".obj", config);

    if (ImGuiFileDialog::Instance()->Display("ChooseObject")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();

            // Windows parser -- TODO resource manager
#ifdef _WIN32
            {
                // make custom path for windows
                std::vector<std::string> words;
                std::string word;
                std::istringstream stream(filePathName);
                bool start_writing = false, add_sep = true;
                string res = "./";

                while (std::getline(stream, word, '\\')) {
                    std::istringstream subStream(word);
                    if (!start_writing && word == "resources") {
                        start_writing = true;
                    }

                    if (start_writing) {
                        res.append(word);
                        res.append("/");
                    }
                }

                if (!res.empty()) {
                    res.pop_back();
                }
                filePathName = string(res);
            }
#endif
            obj_scene.addElement(new Object(filePathName.c_str()), &lightShader);
        }
        // close
        ImGuiFileDialog::Instance()->Close();
        show_object_picker = false;
    }
}

void PlayState::draw(GameEngine *engine) {
    skybox->draw(skyboxShader);
    plane->draw(planeShader);

    obj_scene.draw();

    entityMenu->render();
    modeMenu->render();
    cameraMenu->render();
    lightsMenu->render();
    mousePopup->render();

    if (show_object_picker) {
        showObjectPicker();
    }
}
