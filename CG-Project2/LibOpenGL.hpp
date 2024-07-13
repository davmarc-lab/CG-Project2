#ifndef LIBOPENGL_H
#define LIBOPENGL_H

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#ifdef _WIN32
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#include "dependencies/include/glad/glad.h"
#include "dependencies/include/GLFW/glfw3.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

#include <vector>

static const float WIDTH;
extern const float HEIGHT;

inline void debug(string text) { cout << "-- DEBUG -- " << text << endl; }

inline void warning(string text) { cout << "-- WARNING -- " << text << endl; }

inline void printVec2(vec2 vec) { cout << to_string(vec) << endl; }

inline void printVec3(vec3 vec) { cout << to_string(vec) << endl; }

inline void printMat4(mat4 mat) { cout << to_string(mat) << endl; }

const mat4 textProjection = ortho(0.f, WIDTH, 0.f, HEIGHT);

static mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);

// mmmmm
inline void updatePerspective(float zoom) {
    projection = glm::perspective(glm::radians(zoom), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
}

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

struct Mouse {
    bool first_mouse = true;
    bool active_mode = false;
    float lastX;
    float lastY;
};

#endif