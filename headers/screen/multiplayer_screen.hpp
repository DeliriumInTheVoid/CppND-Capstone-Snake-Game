#pragma once

#include <string>

#include "screen/screen.hpp"
#include "player/player.hpp"


struct Winner;

enum class MultiplayerGameEvent : ScreenEvent
{
    NONE = 0,
    RESTART_GAME,
    MAIN_MENU,
    QUIT_GAME,
};

class MultiplayerGameScreen : public Screen<MultiplayerGameEvent>
{
public:
    MultiplayerGameScreen(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~MultiplayerGameScreen() override = default;

public:
    void Score(const int player1_score, const int player2_score);
    void Init() override;
    virtual void Render() override;
    void SetWinner(const Winner& winner, const int bestScore);

private:
    void UpdateScore(int& score, const int& new_score, std::string& score_text);

private:
    int scorePlayer1{ -1 };
    std::string scoreTextPlayer1;

    int scorePlayer2{ -1 };
    std::string scoreTextPlayer2;

    Winner winner_{};
};
