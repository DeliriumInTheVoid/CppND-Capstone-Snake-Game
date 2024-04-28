#pragma once

#include "SDL_keycode.h"
#include "player/player_id.hpp"
#include "player/player.hpp"
#include "player/a_star_async.hpp"
#include "game_field/cell_type.hpp"


class PlayerAI : public Player
{
public:
    PlayerAI(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight);
    virtual ~PlayerAI() override = default;

public:
    virtual void HandleInput(const SDL_Keycode key_code) override {}

protected:
    bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) override;

private:
    ResultCell nextMoveCell{};
    AStartAsync aStar_;
};
