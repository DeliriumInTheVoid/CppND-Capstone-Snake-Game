#pragma once

#include "game_state/game_states.hpp"
#include "screen/main_menu_screen.hpp"

class MainMenuState : public GameState<MainMenuScreen>
{
public:
    MainMenuState(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override;
    void HandleScreenEvent(const MainMenuEvent event);
};
