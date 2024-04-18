#pragma once

#include "screen/pause_screen.hpp"

#include "game_state/game_states.hpp"


class PauseGameState : public GameState<PauseGameScreen>
{
public:
    PauseGameState(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~PauseGameState() override = default;

public:
    virtual void Update() override;
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override;
    void HandleScreenEvent(const PauseGameEvent event);
    virtual void HandleInput(const SDL_Keycode keyCode) override;
};
