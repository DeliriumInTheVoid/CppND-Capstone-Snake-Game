#include "screen/main_menu_screen.hpp"

MainMenuScreen::MainMenuScreen(const std::size_t screenWidth, const std::size_t screenHeight):
    Screen(screenWidth, screenHeight)
{
}

void MainMenuScreen::Render()
{
    screenEvent_ = MainMenuEvent::NONE;

    const int vGap{ 4 };
    const int buttonsQty{ 4 };

    const ImVec2 windowSize{
        ScreenConsts::BUTTON_SIZE.x,
        ScreenConsts::BUTTON_SIZE.y * buttonsQty + vGap * (buttonsQty - 1)
    };
    const ImVec2 windowPos{ (static_cast<float>(screenWidth_) - windowSize.x) * 0.5f, 200 };
    const auto screenSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(screenSize);

    ImGui::Begin("Game Menu", nullptr,
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoTitleBar
    );

    if (ImGui::Button("Single Player", ScreenConsts::BUTTON_SIZE)) {
        screenEvent_ = MainMenuEvent::SINGLE_PLAYER_GAME;
    }

    if (ImGui::Button("PvP", ScreenConsts::BUTTON_SIZE)) {
        screenEvent_ = MainMenuEvent::PvP_GAME;
    }

    if (ImGui::Button("Player vs AI", ScreenConsts::BUTTON_SIZE)) {
        screenEvent_ = MainMenuEvent::PvAI_GAME;
    }

    if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
        screenEvent_ = MainMenuEvent::QUIT_GAME;
    }

    ImGui::End();
}
