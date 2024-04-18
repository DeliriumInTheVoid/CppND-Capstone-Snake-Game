#pragma once

#include <cstddef>
#include "imgui.h"


class ScreenConsts
{
public:
    static constexpr int BUTTON_WIDTH = 300;
    static constexpr int BUTTON_HEIGHT = 20;
    static constexpr int BUTTONS_VGAP = 40;

    static constexpr ImVec2 BUTTON_SIZE{ BUTTON_WIDTH, BUTTON_HEIGHT };
};

using ScreenEvent = unsigned int;

template <typename T>
class Screen
{
public:
    Screen(const std::size_t screenWidth, const std::size_t screenHeight) :
        screenWidth_{ screenWidth }, screenHeight_{ screenHeight }
    {
    }

    virtual ~Screen() = default;

public:
    T ScreenEvent() const { return screenEvent_; }

public:
    virtual void Render() = 0;
    virtual void Update() {}
    virtual void Init() {}

protected:
    void ShowScoreResult(const int score) const {
        const auto font = ImGui::GetIO().Fonts->Fonts[0];

        if (score > bestScore_ && bestScore_ > 0) {
            font->Scale *= 5.0f;
            ImGui::PushFont(font);
            ImGui::SetCursorPosX(
                (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Congratulations!").x) * 0.5f
            );
            ImGui::Text("Congratulations!");
            ImGui::PopFont();
            font->Scale /= 5.0f;

            font->Scale *= 4.0f;
            ImGui::PushFont(font);
            ImGui::SetCursorPosX(
                (ImGui::GetWindowSize().x - ImGui::CalcTextSize("A New Record: %d").x) * 0.5f
            );
            ImGui::Text("A New Record: %d", score);
            ImGui::SetCursorPosX(
                (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Previous: %d").x) * 0.5f
            );
            ImGui::Text("Previous: %d", bestScore_);
            ImGui::PopFont();
            font->Scale /= 4.0f;

        }
        else {
            font->Scale *= 6.0f;
            ImGui::PushFont(font);
            ImGui::SetCursorPosX(
                (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Score: %d").x) * 0.5f
            );
            ImGui::Text("Score: %d", score);
            ImGui::PopFont();
            font->Scale /= 6.0f;
        }
    }

protected:
    int bestScore_{};
    std::size_t screenWidth_;
    std::size_t screenHeight_;
    T screenEvent_{};
};
