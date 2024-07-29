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
#include "../Menu/IGDebug.hpp"
#include "../Menu/IGEntity.hpp"
#include "../Menu/IGLights.hpp"
#include "../Menu/IGMode.hpp"
#include "../Menu/IGMousePopup.hpp"
#include "../Menu/IGViewport.hpp"

#include "../Menu/Logger/LogManager.hpp"

#include "Game.hpp"
#include "IntroState.hpp"

PlayState PlayState::playState;
string mouse_popup_name = "Menu";

CubeEntity *cube;
Sphere *sphere;
PlaneEntity *plane;
Cubemap *skybox;
Shader lightShader, planeShader, skyboxShader, modelShader;
Object *obj;

DirectionalLight *pl = new DirectionalLight();
SpotLight *ll = new SpotLight();

Entity *obj_selected = nullptr;
Shader *shader_selected = nullptr;

Scene obj_scene;

InputMode user_mode = InputMode::INTERACT;
InputMode old_user_mode = user_mode;

IGMode *modeMenu;
IGCamera *cameraMenu;
IGLights *lightsMenu;
IGEntity *entityMenu = nullptr;
IGMousePopup *mousePopup = new IGMousePopup(mouse_popup_name.c_str());
IGDebug *debugMenu = new IGDebug();
IGViewport *viep = new IGViewport();

bool show_popup = false;
bool show_object_picker = false;
bool simulation_running = false;

bool is_light_selected;
Light *light_selected = nullptr;

Mouse mouse;

string string_buffer = "";

ActionManager *action_manager = ActionManager::instance();
LogManager *debug_log = LogManager::instance();

GLFWwindow* current_context;

void viewportFramebufferCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    viep->rescaleRenderBuffer(width, height);
}

void PlayState::init() {
    debug_log->addLog(logs::INIT, "Start Init PlayState");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    lightShader = Shader("./resources/shaders/lightVertexShader.glsl", "./resources/shaders/lightFragmentShader.glsl");
    Texture cube_texture = Texture("./resources/textures/web-dirt.png");
    cube_texture.createTexture();

    cube = new CubeEntity();
    cube->createVertexArray();
    debug_log->addLog(logs::SILENCE, "Instanced buffers for Cube");
    cube->setPosition(vec3(0, 1, 0));
    cube->setScale(vec3(1));
    cube->attachTexture(cube_texture);
    // cube->setMaterial(material::NONE);

    Texture sphere_texture = Texture("./resources/textures/web-dirt.png");
    sphere_texture.createTexture();

    sphere = new Sphere();
    sphere->createVertexArray();
    debug_log->addLog(logs::SILENCE, "Instanced buffers for Sphere");
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
    debug_log->addLog(logs::SILENCE, "Instanced buffers for Plane");

    skyboxShader = Shader("./resources/shaders/vertexShaderSkybox.glsl", "./resources/shaders/fragmentShaderSkybox.glsl");
    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setInt("skybox", 0);

    skybox = new Cubemap();
    skybox->createVertexArray();
    debug_log->addLog(logs::SILENCE, "Instanced buffers for Skybox");

    // add elements to the scene
    obj_scene.addElement(cube, &lightShader);
    obj_scene.addElement(sphere, &lightShader);

    ll->initCaster();
    ll->setPosition(vec3(1));
    ll->setDirection(vec3(0, 1, 0));

    // obj_scene.addLight(pl);
    obj_scene.addLight(ll);

    modelShader = Shader("./resources/shaders/lightVertexShader.glsl", "./resources/shaders/lightFragmentShader.glsl");
    modelShader.use();
    modelShader.setMat4("projection", projection);

    obj = new Object("./resources/models/backpack/backpack.obj", Flip::VERTICALLY);
    debug_log->addLog(logs::SILENCE, "Instanced buffers for imported Object");
    obj->setPosition(vec3(0));
    obj->setScale(vec3(0.5));
    obj_scene.addElement(obj, &lightShader);

    // imgui
    debug_log->addLog(logs::INIT, "Start Init ImGui menus");

    entityMenu = new IGEntity();
    modeMenu = new IGMode(&user_mode);
    cameraMenu = new IGCamera();
    lightsMenu = new IGLights(obj_scene.getLights());
    debug_log->addLog(logs::INIT, "End Init ImGui menus");

    debug_log->addLog(logs::INIT, "End Init PlayState");
    viep->init();
}

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

            // distance between ray and object
            float dist = 0.f;

            // check all the element of the scene for intersection with ray
            for (auto elem : obj_scene.getElements()) {
                auto current = elem.first;
                auto shader = elem.second;

                if (isRayInSphere(ray, current->getPosition(), 0.7, &dist)) {
                    if (obj_selected == nullptr || dist <= ci) {
                        obj_selected = current;
                        shader_selected = shader;
                        ci = dist;
                        is_light_selected = false;
                        light_selected = nullptr;
                    }
                }
            }

            debug_log->addLog(logs::ERROR, "Sphere radius value (0.1f) could be incorrect");
            for (auto caster : obj_scene.getLights()) {
                if (caster->getType() != LightType::DIRECTIONAL && isRayInSphere(ray, caster->getCaster()->getPosition(), 0.1f, &dist)) {
                    if (obj_selected == nullptr || dist <= ci) {
                        obj_selected = caster->getCaster();
                        shader_selected = caster->getShader();
                        ci = dist;
                        is_light_selected = true;
                        light_selected = caster;
                    }
                }
            }

            // updating the observer in IGEntity menu
            if (obj_selected != nullptr) {
                entityMenu->changeObserver(obj_selected, shader_selected);
                debug_log->addLog(logs::SELECT_WARNING, "Working on the same shader, it affects all the shaders");
            } else {
                entityMenu->resetObserver();
                debug_log->addLog(logs::SELECT_WARNING, "No Entity Selected");
            }
        }
    }

    imGuiMouse2Popup();

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mod);
}

void scrollPosFunc(GLFWwindow *window, double xoffset, double yoffset) { ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset); }

void PlayState::handleEvent(GameEngine *engine) {
    current_context = engine->getWindow()->getGLFWContext();
    const float dt = engine->getDeltaTime();

    // Change mode
    if (glfwGetKey(current_context, GLFW_KEY_G) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_G)) {
        user_mode = InputMode::INTERACT;
    }
    if (glfwGetKey(current_context, GLFW_KEY_V) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_V)) {
        user_mode = InputMode::SELECT;
    }
    if (glfwGetKey(current_context, GLFW_KEY_P) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_P)) {
        user_mode = InputMode::PASSIVE;
    }
    if (glfwGetKey(current_context, GLFW_KEY_Q) == GLFW_PRESS || ImGui::IsKeyPressed(ImGuiKey_Q)) {
        // This changeState kinda work for PlayState but need to test more
        // engine->changeState(IntroState::instance());
        debug_log->addLog(logs::GENERAL_EVENT, "Game Closing");
        // Print all logs in a txt file
        engine->quit();
    }

    if (user_mode != old_user_mode) {
        old_user_mode = user_mode;
        switch (user_mode) {
        case SELECT:
            // enable selecting object mode
            debug_log->addLog(logs::USER_MODE, "User Mode -> SELECT");
            mouse.first_mouse = true;
            glfwSetInputMode(current_context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetMouseButtonCallback(current_context, selectMouseFunc);
            glfwSetCursorPosCallback(current_context, mouseActiveMotion);
            glfwSetScrollCallback(current_context, scrollPosFunc);
            break;
        case INTERACT:
            // enable movement in the scene, and active mouse movement
            debug_log->addLog(logs::USER_MODE, "User Mode -> INTERACT");
            mouse.first_mouse = true;
            glfwSetInputMode(current_context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetMouseButtonCallback(current_context, mouseInputFunc);
            glfwSetCursorPosCallback(current_context, mouseActiveMotion);
            glfwSetScrollCallback(current_context, scrollPosFunc);
            break;
        case PASSIVE:
            // enable passive mouse movement
            debug_log->addLog(logs::USER_MODE, "User Mode -> PASSIVE");
            mouse.first_mouse = true;
            glfwSetInputMode(current_context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(current_context, passiveCursorPosFunc);
            glfwSetMouseButtonCallback(current_context, clearMouseActionFunc);

            if (glfwRawMouseMotionSupported())
                glfwSetInputMode(current_context, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

            break;
        default:
            break;
        }
    }

    // Mouse input
    double x, y;
    glfwGetCursorPos(current_context, &x, &y);
    y = HEIGHT - y;
    vec2 mouse_pos = vec2(x, y);

    // Camera input
    float cameraVelocity = camera.getCameraVelocity() * engine->getDeltaTime();
    if (glfwGetKey(current_context, GLFW_KEY_W) == GLFW_PRESS) {
        // lockKey(GLFW_KEY_W);
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_S) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraFront();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_A) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_D) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraRight();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_SPACE) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() + cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        auto pos = camera.getCameraPosition() - cameraVelocity * camera.getCameraUp();
        camera.moveCamera(pos);
    }
    if (glfwGetKey(current_context, GLFW_KEY_DELETE) == GLFW_PRESS) {
        am->addAction(Action::DEL_ENTITY);
    }
}

void PlayState::update(GameEngine *engine) {
    glfwSetFramebufferSizeCallback(engine->getWindow()->getGLFWContext(), viewportFramebufferCallback);
    if (action_manager->isActionPresent()) {
        for (auto act : action_manager->getActions()) {
            switch (act) {
            case Action::ADD_CUBE_ENTITY:
                debug_log->addLog(logs::ADD_ENTITY, "Added Cube to Scene");
                obj_scene.addElement(new CubeEntity(), &lightShader);
                break;
            case Action::ADD_SPHERE_ENTITY:
                debug_log->addLog(logs::ADD_ENTITY, "Added Sphere to Scene");
                obj_scene.addElement(new Sphere(), &lightShader);
                break;
            case Action::ADD_OBJECT_ENTITY: {
                debug_log->addLog(logs::ADD_ENTITY, "Added extern Object to Scene");
                show_object_picker = true;
                break;
            }
            case Action::ADD_DIRECT_LIGHT:
                obj_scene.addLight(new DirectionalLight());
                debug_log->addLog(logs::ADD_ENTITY, "Added Directional Light to Scene");
                lightsMenu->refreshLights(obj_scene.getLights());
                break;
            case Action::ADD_POINT_LIGHT:
                debug_log->addLog(logs::ADD_ENTITY, "Added Point Light to Scene");
                obj_scene.addLight(new PointLight());
                lightsMenu->refreshLights(obj_scene.getLights());
                break;
            case Action::ADD_SPOT_LIGHT: {
                debug_log->addLog(logs::ADD_ENTITY, "Added Spot Light to Scene");
                obj_scene.addLight(new SpotLight());
                lightsMenu->refreshLights(obj_scene.getLights());
                break;
            }
            case Action::DEL_ENTITY:
                if (obj_selected != nullptr) {
                    debug_log->addLog(logs::REMOVE_ENTITY, "Removed Entity from Scene");
                    obj_scene.removeElement(obj_selected, shader_selected, is_light_selected, light_selected);
                    obj_selected = nullptr;
                    shader_selected = nullptr;
                    light_selected = nullptr;
                    entityMenu->resetObserver();
                    lightsMenu->refreshLights(obj_scene.getLights());
                }
                break;
            case Action::DEL_CUSTOM_LIGHT:
                cout << obj_scene.removeElement(nullptr, nullptr, true, lightsMenu->getLightToDelete()) << endl;
                lightsMenu->refreshLights(obj_scene.getLights());
                break;
            case Action::REFRESH_PROJ:
                planeShader.use();
                planeShader.setMat4("projection", projection);

                skyboxShader.use();
                skyboxShader.setMat4("projection", projection);
            case Action::START_SIM: {
                simulation_running = true;
                break;
            }
            case Action::STOP_SIM: {
                simulation_running = false;
                break;
            }
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
    if (simulation_running) {
        viep->bind();
    }
    skybox->draw(skyboxShader);
    plane->draw(planeShader);

    obj_scene.draw();
    if (simulation_running) {
        viep->unbind();
        viep->render();
    }

    entityMenu->render();
    modeMenu->render();
    cameraMenu->render();
    lightsMenu->render();
    mousePopup->render();

    if (show_object_picker) {
        showObjectPicker();
    }

    debugMenu->render();
}

void PlayState::clean() {
    // delete obj_selected;
    // delete shader_selected;
    delete plane;
    delete skybox;
    delete entityMenu;
    delete modeMenu;
    delete cameraMenu;
    delete lightsMenu;
    delete mousePopup;
    delete debugMenu;
}
