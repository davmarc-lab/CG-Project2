#include "Lib.hpp"
#include "Window/Window.hpp"
#include "Game/Game.hpp"

#include "Entity/Entity.hpp"
#include "Component/Transform.hpp"
#include "System/TransformSystem.hpp"

#define GAME_NAME "Welcome 3D"

const int WIDTH = 1812, HEIGHT = 1017;

Window window = Window(GAME_NAME, WIDTH, HEIGHT);
Game game = Game(WIDTH, HEIGHT);

// time variables 
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/*
 *  Callback function for keyboard user input
 *
 *  @param key keyboard key pressed
 *  @param scancode scancode of the key
 *  @param action GLFW action for keys
 *  @param mods tells which special key where pressed
 */
void keyboard_input_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    game.processInput(key, action, deltaTime);
}

int main() {

    /* Entity ent; */
    /* Transform comp(vec3(25, 500, 1000), vec3(1), vec3(90, -90, 0)); */

    /* ent.addComponent<Transform>(comp); */
    /* TransformSystem system = TransformSystem(); */
    /* system.addEntity(&ent); */
    /* system.update(); */

    /* ent.getComponent<Transform>()->setPosition(vec3(0, 0, 0)); */
    /* system.update(); */
    
    // initialize game entities
    game.init();


    // Start of window loop
    while (!glfwWindowShouldClose(window.getWindow())) {
        // manage frames time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        window.processCloseInput();
        glfwSetKeyCallback(window.getWindow(), keyboard_input_callback);

        // update game state
        game.update(deltaTime);

        // render
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    window.terminateWindow();
    return 0;
}
