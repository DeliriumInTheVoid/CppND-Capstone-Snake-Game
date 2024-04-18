#pragma once

#include "imgui.h"
#include "screen/screen.hpp"


enum class PauseGameEvent : ScreenEvent
{
    NONE = 0,
    RESUME_GAME,
    QUIT_GAME,
};

class PauseGameScreen : public Screen<PauseGameEvent>
{
public:
    PauseGameScreen(const std::size_t screenWidth, const std::size_t screenHeight) :
        Screen(screenWidth, screenHeight)
    {}

    virtual ~PauseGameScreen() override = default;

    virtual void Render() override
    {
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
};
