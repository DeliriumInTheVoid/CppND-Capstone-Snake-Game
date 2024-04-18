#pragma once

#include <memory>
#include "SDL.h"

#include "game_field/game_field.hpp"
#include "game_field/gamefield_factory.hpp"
#include "game_field/game_field_state.hpp"


enum class GameStateType: unsigned;

class GamePlayManager
{
public:
    GamePlayManager(std::unique_ptr<GameFieldFactory>&& gameFieldFactory);
    ~GamePlayManager() = default;

public:
    const std::unique_ptr<GameField>& gameField() const;

public:
    void ChangeState(const GameStateType nextStateType);
    void HandleInput(const SDL_Keycode keyCode) const;
    GameFieldState Update() const;
    void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) const;

private:
    GameStateType stateType_{ GameStateType::NONE };
    std::unique_ptr<GameFieldFactory> gameFieldFactory_;
    std::unique_ptr<GameField> gameField_;
};
