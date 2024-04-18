#pragma once

#include <memory>

#include "game_field/game_field.hpp"
#include "game_state/game_state_type.hpp"



class GameFieldFactory
{
public:
    GameFieldFactory(const std::size_t gridWidth, const std::size_t gridHeight);
    ~GameFieldFactory() = default;

public:
    std::unique_ptr<GameField> CreateGameField(const GameStateType type);

private:
    std::size_t gridWidth_;
    std::size_t gridHeight_;
};
