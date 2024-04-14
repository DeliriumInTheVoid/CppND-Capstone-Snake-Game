#pragma once

#include <memory>

#include "game_states.hpp"

class GameStateFactory
{
public:
    GameStateFactory(const std::size_t screenWidth, const std::size_t screenHeight)
        : screenWidth_{ screenWidth }, screenHeight_{ screenHeight }
    {}

    ~GameStateFactory() = default;

public:
    std::unique_ptr<GameStateBase> CreateGameState(const GameStateType type) {
        switch (type) {
            case GameStateType::MAIN_MENU:
                return std::make_unique<MainMenuState>(screenWidth_, screenHeight_);
            case GameStateType::SINGLE_PLAYER_GAME:
                return std::make_unique<SinglePlayerGameState>(screenWidth_, screenHeight_);
        case GameStateType::PvP_GAME:
                return std::make_unique<MultiplayerGameState>(screenWidth_, screenHeight_, GameStateType::PvP_GAME);
        case GameStateType::PvAI_GAME:
                return std::make_unique<MultiplayerGameState>(screenWidth_, screenHeight_, GameStateType::PvAI_GAME);
            case GameStateType::PAUSE_GAME:
                return std::make_unique<PauseGameState>(screenWidth_, screenHeight_);
            default:
                return nullptr;
        }
    }

private:
    std::size_t screenWidth_;
    std::size_t screenHeight_;
};
