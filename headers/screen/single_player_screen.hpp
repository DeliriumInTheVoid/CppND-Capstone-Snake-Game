#pragma once

#include <string>

#include "screen/screen.hpp"


enum class SinglePlayerGameEvent : ScreenEvent
{
    NONE = 0,
    RESTART_GAME,
    MAIN_MENU,
    QUIT_GAME,
};

class SinglePlayerGameScreen : public Screen<SinglePlayerGameEvent>
{
public:
    SinglePlayerGameScreen(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~SinglePlayerGameScreen() override = default;

public:
    void Score(const int score);
    void Init() override;
    void GameOver(const int bestScore);
    virtual void Render() override;

private:
    bool gameOver_{ false };
    int score_{ -1 };
    std::string scoreText_;
};
