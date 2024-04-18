#pragma once

#include "game_state/game_states.hpp"
#include "screen/single_player_screen.hpp"


class SinglePlayerGameState : public GameState<SinglePlayerGameScreen>
{
public:
    SinglePlayerGameState(const std::size_t screenWidth, const std::size_t screenHeight);
    virtual ~SinglePlayerGameState() override = default;

public:
    virtual void Init() override;
    virtual void Update() override;
    virtual void HandleInput(const SDL_Keycode keyCode) override;
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override;
    void HandleScreenEvent(const SinglePlayerGameEvent event);

protected:
    void HandleGameFieldStateChange(const GameFieldState state) override;
};
