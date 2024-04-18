#pragma once

#include <unordered_map>

#include "SDL.h"

#include "player/player.hpp"
#include "game_field_state.hpp"

class GameField
{
public:
    GameField(const std::size_t gridWidth, const std::size_t gridHeight)
        : gridWidth_{ gridWidth }
        , gridHeight_{ gridHeight }
    {}
    virtual ~GameField() {}

public:
    virtual void Init() = 0;
    virtual GameFieldState Update() = 0;
    virtual void Render(SDL_Renderer* sdlRenderer, SDL_Rect& block) = 0;
    virtual void HandleInput(SDL_Keycode keyCode) = 0;

    virtual int PlayerScore(PlayerId player_id) const = 0;

    virtual Winner GetWinner() const { return {}; }

public:
    GameFieldState GameState() const { return gameFieldState_; }

    void AddPlayer(std::unique_ptr<Player>&& player) {
        players_.emplace(player->Id(), std::move(player));
    }

protected:
    std::size_t gridWidth_;
    std::size_t gridHeight_;

    GameFieldState gameFieldState_{ GameFieldState::NONE };
    std::unordered_map<PlayerId, std::unique_ptr<Player>> players_{};
};
