#include "imgui.h"
#include "screen/pause_screen.hpp"


PauseGameScreen::PauseGameScreen(const std::size_t screenWidth, const std::size_t screenHeight):
    Screen(screenWidth, screenHeight)
{}

void PauseGameScreen::Render() {
    screenEvent_ = PauseGameEvent::NONE;

    ImGui::Begin("Game Menu", nullptr,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoTitleBar
    );

    if (ImGui::Button("Resume", ImVec2(ScreenConsts::BUTTON_WIDTH, 0))) {
        screenEvent_ = PauseGameEvent::RESUME_GAME;
    }

    if (ImGui::Button("Main Menu", ImVec2(ScreenConsts::BUTTON_WIDTH, 0))) {
        screenEvent_ = PauseGameEvent::QUIT_GAME;
    }

    ImGui::End();
}
