#include "screen/single_player_screen.hpp"

SinglePlayerGameScreen::SinglePlayerGameScreen(const std::size_t screenWidth, const std::size_t screenHeight):
    Screen(screenWidth, screenHeight)
{}

void SinglePlayerGameScreen::Score(const int score) {
    if (score_ == score) {
        return;
    }
    score_ = score;
    scoreText_ = "Score: " + std::to_string(score);
}

void SinglePlayerGameScreen::Init() {
    Score(0);
    gameOver_ = false;
}

void SinglePlayerGameScreen::GameOver(const int bestScore) {
    gameOver_ = true;
    bestScore_ = bestScore;
}

void SinglePlayerGameScreen::Render() {
    screenEvent_ = SinglePlayerGameEvent::NONE;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Score Label", nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoBackground |
                 ImGuiWindowFlags_NoSavedSettings
    );
    ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - ImGui::CalcTextSize(scoreText_.c_str()).x) / 2, 10));
    ImGui::Text("%s", scoreText_.c_str());

    if (gameOver_) {
        const auto sW = static_cast<float>(screenWidth_);
        ImGui::SetCursorPos({ (sW - ScreenConsts::BUTTON_WIDTH) / 2, 100 });
        if (ImGui::Button("Restart Game", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = SinglePlayerGameEvent::RESTART_GAME;
        }

        ImGui::SetCursorPosX((sW - ScreenConsts::BUTTON_WIDTH) / 2);
        if (ImGui::Button("Main Menu", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = SinglePlayerGameEvent::MAIN_MENU;
        }

        ImGui::SetCursorPosX((sW - ScreenConsts::BUTTON_WIDTH) / 2);
        if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = SinglePlayerGameEvent::QUIT_GAME;
        }

        const auto font = ImGui::GetIO().Fonts->Fonts[0];
        //auto const fontSize = font->FontSize;
        //font->FontSize = 28;
        font->Scale *= 8.0f;
        ImGui::PushFont(font);
        ImGui::SetCursorPosX(
            (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Game Over").x) * 0.5f
        );
        ImGui::Text("Game Over");
        ImGui::PopFont();
        //font->FontSize = fontSize;
        font->Scale /= 8.0f;

        ShowScoreResult(score_);
    }

    ImGui::End();
}
