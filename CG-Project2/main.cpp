#include "Component/IComponent.hpp"
#include "Entity/Entity.hpp"
#include "Entity/Object.hpp"
#include "Lib.hpp"
#include "Window/Window.hpp"

#include <iostream>

#define PI 3.14159265358979323846
#define GAME_NAME "Welcome 3D"

const int WIDTH = 1812, HEIGHT = 1017;

int main() {
    Window window = Window(GAME_NAME, WIDTH, HEIGHT);

    /* Window second = Window(GAME_NAME, WIDTH, HEIGHT); */
    /* const std::type_info& gg = typeid(Window); */
    /* cout << gg.name() << ", " << typeid(window).name() << endl; */

    Entity cube = Entity();

    Object *obj = new Object(vec3(0), vec3(1), vec3(0));

    cout << obj->getId() << endl;

    // time variables 
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Start of window loop
    while (!glfwWindowShouldClose(window.getWindow())) {
        // manage frames time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        window.processCloseInput();

        // update game state

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    window.terminateWindow();
    return 0;
}
