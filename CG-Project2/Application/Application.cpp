#include "Application.hpp"

#include "../LibCustomGui.hpp"

#include "../Game/Game.hpp"
#include "../Game/IntroState.hpp"
#include "../Window/Window.hpp"

#include "../Menu/Logger/LogManager.hpp"

void Application::launch() {
    // instanciate window
    Window window = Window(this->name.c_str(), this->width, this->height);
    GameEngine game = GameEngine(&window, this->width, this->height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(),
                                 true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    game.init();

    LogManager::instance()->addLog(logs::STATE, glfwGetTime(), "Creating IntroState");
    game.changeState(IntroState::instance());
    LogManager::instance()->addLog(logs::STATE, glfwGetTime(), "Instanced IntroState");
    float lastFrame = 0;

    while (!glfwWindowShouldClose(window.getWindow()) && game.isRunning()) {
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glfwPollEvents();
        float currentFrame = glfwGetTime();
        game.setDeltaTime(currentFrame - lastFrame);
        lastFrame = currentFrame;

        // handle input
        game.processInput();

        // update game
        game.update();

        // render game
        game.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window.getWindow());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminateWindow();
    game.clean();
}
