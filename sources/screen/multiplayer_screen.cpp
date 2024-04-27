#include "screen/multiplayer_screen.hpp"


MultiplayerGameScreen::MultiplayerGameScreen(const std::size_t screenWidth, const std::size_t screenHeight) :
    Screen(screenWidth, screenHeight)
{}

void MultiplayerGameScreen::Score(const int player1_score, const int player2_score)
{
    UpdateScore(scorePlayer1, player1_score, scoreTextPlayer1);
    UpdateScore(scorePlayer2, player2_score, scoreTextPlayer2);
}

void MultiplayerGameScreen::Init()
{
    winner_ = {};
    screenEvent_ = MultiplayerGameEvent::NONE;
    Score(0, 0);
}

void MultiplayerGameScreen::Render() {
    screenEvent_ = MultiplayerGameEvent::NONE;

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

    ImGui::SetCursorPos(ImVec2(10, 10));
    ImGui::Text("%s", scoreTextPlayer1.c_str());

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize(scoreTextPlayer2.c_str()).x - 10, 10));
    ImGui::Text("%s", scoreTextPlayer2.c_str());

    if (winner_.id != PlayerId::UNKNOWN) {
        const auto sW = static_cast<float>(screenWidth_);
        ImGui::SetCursorPos({ (sW - ScreenConsts::BUTTON_WIDTH) / 2, 100 });
        if (ImGui::Button("Restart Game", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MultiplayerGameEvent::RESTART_GAME;
        }

        ImGui::SetCursorPosX((sW - ScreenConsts::BUTTON_WIDTH) / 2);
        if (ImGui::Button("Main Menu", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MultiplayerGameEvent::MAIN_MENU;
        }

        ImGui::SetCursorPosX((sW - ScreenConsts::BUTTON_WIDTH) / 2);
        if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MultiplayerGameEvent::QUIT_GAME;
        }

        const auto font = ImGui::GetIO().Fonts->Fonts[0];

        const auto winnerText = std::string{ winner_.name } + " Wins!";

        font->Scale *= 6.0f;
        ImGui::PushFont(font);
        ImGui::SetCursorPosX(
            (ImGui::GetWindowSize().x - ImGui::CalcTextSize(winnerText.c_str()).x) * 0.5f
        );
        ImGui::Text(winnerText.c_str());
        ImGui::PopFont();
        font->Scale /= 6.0f;

        ShowScoreResult(winner_.score);
    }

    ImGui::End();
}

void MultiplayerGameScreen::SetWinner(const Winner& winner, const int bestScore) {
    winner_ = winner;
    bestScore_ = bestScore;
}

void MultiplayerGameScreen::UpdateScore(int& score, const int& new_score, std::string& score_text) {

    if (new_score == score) {
        return;
    }

    score = new_score;
    score_text = "Score: " + std::to_string(score);
}
