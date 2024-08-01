#include "Application.hpp"

#include "../LibCustomGui.hpp"

#include "../Game/Game.hpp"
#include "../Game/IntroState.hpp"
#include "../Window/Window.hpp"

#include "../Menu/Logger/LogManager.hpp"

void Application::launch() {
    // instanciate window
    Window window = Window(this->name.c_str(), this->width, this->height);
    // instanciate the engine
    GameEngine game = GameEngine(&window, this->width, this->height);

    // Creates imgui context with all necessary config flags.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking Feature
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable MultiViewport Feature

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getGLFWContext(),
                                 true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    game.init();

    LogManager::instance()->addLog(logs::STATE, "Creating IntroState");
    // Sets the first GameState for the game loop (the start up menu)
    game.changeState(IntroState::instance());
    LogManager::instance()->addLog(logs::STATE, "Instanced IntroState");

    // These variables are used to calculate the deltaTime between every frame.
    float lastFrame = 0;
    float current_frame = 0;

    // backup window for imgui
    GLFWwindow *backup_current_context;

    // Starting the game loop until the GLFWwindow closes or the engine stops
    while (!glfwWindowShouldClose(window.getGLFWContext()) && game.isRunning()) {
        // Defines the GLFWwindow clear color (background color)
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        // Sets some flags for rendering purpose: enables the color buffer and the depth buffer for 3D
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Manages ImGui window every frame to adjust internal parameters
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Polls all GLFW events like keyboard input, mouse input, etc.
        glfwPollEvents();
        current_frame = glfwGetTime();
        game.setDeltaTime(current_frame - lastFrame);
        lastFrame = current_frame;

        // handle input
        game.processInput();

        // update game
        game.update();

        // render game
        game.render();

        // render imgui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // create backup GLFWwindow context to avoid graphic glitches due to ImGui Viewport
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        // Render the current buffer used and prepare the other one.
        glfwSwapBuffers(window.getGLFWContext());
    }

    // Clear all ImGui objects
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Clear GLFWwindow and the game itself
    window.terminateWindow();
    game.clean();
}
