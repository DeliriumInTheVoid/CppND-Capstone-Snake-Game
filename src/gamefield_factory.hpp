#pragma once

#include <memory>

#include "game_field.hpp"
#include "game_state_type.hpp"

class GameFieldFactory
{
public:
    GameFieldFactory(std::size_t gridWidth, std::size_t gridHeight)
        : gridWidth_{gridWidth}
        , gridHeight_{gridHeight}
    {}

    ~GameFieldFactory() = default;

public:
    std::unique_ptr<GameField> createGameField(GameStateType type) {
        switch (type)
        {
            case GameStateType::SINGLE_PLAYER_GAME:
            {
                auto gameField = std::make_unique<PlayerGameField>(gridWidth_, gridHeight_);
                gameField->AddPlayer(std::make_unique<Player>(PlayerId::PLAYER_1, gridWidth_, gridHeight_));
                return gameField;
            }
            case GameStateType::PvP_GAME:
            {
                auto gameField = std::make_unique<PlayerGameField>(gridWidth_, gridHeight_);
                gameField->AddPlayer(std::make_unique<Player>(PlayerId::PLAYER_1, gridWidth_, gridHeight_));
                gameField->AddPlayer(std::make_unique<Player>(PlayerId::PLAYER_2, gridWidth_, gridHeight_));
                return gameField;
            }
            default:
                return std::make_unique<NullGameField>();
        }
    }
private:
    std::size_t gridWidth_;
    std::size_t gridHeight_;
};
