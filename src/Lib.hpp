#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;
using namespace glm;

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
