#pragma once

#include "screen/screen.hpp"

enum class MainMenuEvent : ScreenEvent
{
    NONE = 0,
    MAIN_MENU,
    SINGLE_PLAYER_GAME,
    PvP_GAME,
    PvAI_GAME,
    QUIT_GAME,
};


class MainMenuScreen : public Screen<MainMenuEvent>
{
public:
    MainMenuScreen(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~MainMenuScreen() override = default;

public:
    virtual void Render() override;
    virtual void Update() override {}
};
