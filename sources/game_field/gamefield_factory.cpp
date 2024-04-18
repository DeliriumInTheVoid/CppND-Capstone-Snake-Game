#include "game_field/gamefield_factory.hpp"
#include "game_field/null_game_field.hpp"
#include "game_field/player_game_field.hpp"

#include "player/ai_player.hpp"


GameFieldFactory::GameFieldFactory(const std::size_t gridWidth, const std::size_t gridHeight) : gridWidth_{ gridWidth }
, gridHeight_{ gridHeight }
{}

std::unique_ptr<GameField> GameFieldFactory::CreateGameField(const GameStateType type) {
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
    case GameStateType::PvAI_GAME:
    {
        auto gameField = std::make_unique<PlayerGameField>(gridWidth_, gridHeight_);
        gameField->AddPlayer(std::make_unique<Player>(PlayerId::PLAYER_1, gridWidth_, gridHeight_));
        gameField->AddPlayer(std::make_unique<PlayerAI>(PlayerId::PLAYER_2, gridWidth_, gridHeight_));
        return gameField;
    }
    default:
        return std::make_unique<NullGameField>();
    }
}
