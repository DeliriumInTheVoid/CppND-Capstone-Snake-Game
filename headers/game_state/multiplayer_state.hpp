#pragma once

#include "screen/multiplayer_screen.hpp"

#include "game_state/game_states.hpp"


class MultiplayerGameState : public GameState<MultiplayerGameScreen>
{
public:
    MultiplayerGameState(const std::size_t screenWidth, const std::size_t screenHeight, const GameStateType stateType);

public:
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override;
    void HandleScreenEvent(const MultiplayerGameEvent event);
    virtual void HandleInput(SDL_Keycode keyCode) override;
    void HandleGameFieldStateChange(const GameFieldState state) override;
};
