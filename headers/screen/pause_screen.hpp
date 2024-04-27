#pragma once

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
    PauseGameScreen(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~PauseGameScreen() override = default;

public:
    virtual void Render() override;
};
