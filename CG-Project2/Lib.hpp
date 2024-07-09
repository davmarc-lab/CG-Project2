#pragma once

#include "Camera/Camera.hpp"
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL

#ifdef _WIN32
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#include "dependencies/include/glad/glad.h"
#include "dependencies/include/GLFW/glfw3.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;
using namespace glm;

inline void debug(string text) {
    cout << "-- DEBUG -- " << text << endl;
}

inline void warning(string text) {
    cout << "-- WARNING -- " << text << endl;
}

inline void printVec3(vec3 vec) {
    cout << to_string(vec) << endl;
}

inline void printVec2(vec2 vec) {
    cout << to_string(vec) << endl;
}

inline void printMat4(mat4 mat) {
    cout << to_string(mat) << endl;
}

const float WIDTH = 1600.f, HEIGHT = 900.f;

const mat4 textProjection = ortho(0.f, WIDTH, 0.f, HEIGHT);

const mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);

const float BLOCK_DIM = 0.1f;
const float OUTLINE_DIM = 0.001f;
const float MARGIN_CUBE = -0.01f;
const float OFFSET_CUBE = 0.000001f;

static Camera camera = Camera();

struct Buffers {
    GLuint vao;
    GLuint vbo_g;
    GLuint vbo_c;
    GLuint vbo_t;
    GLuint vbo_n;
    GLuint ebo;
};

struct Coords {
    vector<vec3> vertex;
    vector<vec4> colors;
    vector<vec2> texCoords;
    vector<vec3> normals;
};

struct Model {
    vec3 position = vec3(0);
    vec3 scale = vec3(1);
    vec3 rotation = vec3(0);
};

struct BoundingBox {
    vec3 botLeft = vec3(2);
    vec3 topRight = vec3(-2);
};

struct Physic {
    vec3 velocity = vec3(0);
    vec3 acceleration = vec3(0);
    vec3 force = vec3(0);
    float mass = 1.f;

    float motionTime = 0.f;
};

enum InputMode {
    SELECT,
    INTERACT,
};

inline string calcInputMode(InputMode mode) {
    switch (mode)
    {
    case SELECT:
        return "Select";
    case INTERACT:
        return "Interact";
    default:
        return "None";
    }
}
