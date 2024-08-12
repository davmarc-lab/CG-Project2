#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "Camera/Camera.hpp"

// Libraries path for Visual Studio solution
#ifdef _WIN32
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else // Libraries path using terminal
#include "dependencies/include/glad/glad.h"
#include "dependencies/include/GLFW/glfw3.h"
#endif

// glm must be installed in the system if not using VS solution
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

// Useful methods to fast print values
inline void debug(string text) { cout << "-- DEBUG -- " << text << endl; }

inline void warning(string text) { cout << "-- WARNING -- " << text << endl; }

inline void printVec3(vec3 vec) { cout << to_string(vec) << endl; }

inline void printVec2(vec2 vec) { cout << to_string(vec) << endl; }

inline void printMat4(mat4 mat) { cout << to_string(mat) << endl; }

// Primary Window dimension
const float WIDTH = 1200.f, HEIGHT = 700.f;

// Default text projection matrix
const mat4 textProjection = ortho(0.f, WIDTH, 0.f, HEIGHT);

// Global Projection Matrix
inline mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);

// Method used to update the perspective matrix if the camera zoom is modified.
inline void updatePerspective(float zoom) {
      projection = glm::perspective(glm::radians(zoom), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
}

// Initial max number lights computed in shaders
inline int max_lights = 4;

inline Camera camera = Camera();

// Struct containing all common OpenGl buffers.s
struct Buffers {
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;
    GLuint vbo_t;
    GLuint vbo_n;
    GLuint ebo;
};

// Struct containing all common vertex information.
struct Coords {
    vector<vec3> vertex;
    vector<vec4> colors;
    vector<vec2> texCoords;
    vector<vec3> normals;
};

// Struct containing each Entity world information.
struct Model {
    vec3 position = vec3(0);
    vec3 scale = vec3(1);
    vec3 rotation = vec3(0);
};

// AABB Collision System not used in this simulation (but implementation is written)
struct BoundingBox {
    vec3 botLeft = vec3(2);
    vec3 topRight = vec3(-2);
};

// Struct containing mouse information.
struct Mouse {
    bool first_mouse = true;
    bool active_mode = false;
    float lastX;
    float lastY;
};

struct Physic {
    float mass = 1.0f;
    float velocity = 0.0f;
    vec3 acceleration = vec3(0);
    vec3 force = vec3(0);
};

// PlayState Input modes.
enum InputMode {
    // in select mode user can select objects
    SELECT,
    // in interact mode user can move in the scene and use imgui windows
    INTERACT,
    // in passive mode user can move camera using passive mouse motion
    PASSIVE,
    EMPTY
};

// Retrieves the String associated to each InputMode.
inline string calcInputMode(InputMode mode) {
    switch (mode) {
    case SELECT:
        return "Select";
    case INTERACT:
        return "Interact";
    case PASSIVE:
        return "Passive";
    default:
        return "None";
    }
}
