#include "Color/Color.hpp"
#include "Lib.hpp"
#include "Menu/Menu.hpp"
#include "Menu/StartMenu.hpp"
#include "Game/Game.hpp"
#include "Menu/ButtonComponent.hpp"
#include "Menu/Component.hpp"
#include "Menu/RectComponent.hpp"
#include "Text/Text.hpp"
#include "Window/Window.hpp"

#define GAME_NAME "3D Project"

int main() {
    Window window = Window(GAME_NAME, WIDTH, HEIGHT);
    Menu* menu = new StartMenu(window.getWindow());
    // Initialize all game object and window

    ButtonComponent* c = new ButtonComponent(vec3(310, 310, 0), color::WHITE, "Options", 40, color::BLUE);
    c->setPosition(vec3(310, 310, 0));
    c->setScale(vec3(100, 30, 0));
    c->createVertexArray();

    // time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while(!glfwWindowShouldClose(window.getWindow())
        && !(menu->getState() == MenuState::PLAY 
        || menu->getState() == MenuState::EXIT)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        window.processCloseInput();
        menu->processMouseInput(deltaTime);

        // update menu state

        double x;
        double y;
        glfwGetCursorPos(window.getWindow(), &x, &y);
        y = HEIGHT - y;

        // render
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        menu->render();

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    menu->clear();

    if (menu->getState() == EXIT) {
        window.terminateWindow();
        exit(0);
    }

    // Let's play


    // enable depth test for 3D
    window.enableGL(GL_DEPTH_TEST);
    // Start of game loop

    Game game = Game(WIDTH, HEIGHT);
    game.init(&window);

    while (!glfwWindowShouldClose(window.getWindow())) {
        // manage frames time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        window.processCloseInput();
        game.processInput(deltaTime, window);

        // update game state
        game.update(deltaTime);

        /* double x;
        double y;
        glfwGetCursorPos(window.getWindow(), &x, &y);
        y = HEIGHT - y; */

        // render
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.render();

        // swap buffers and poll IO events
        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }
    game.clear();
    window.terminateWindow();
    return 0;
}
