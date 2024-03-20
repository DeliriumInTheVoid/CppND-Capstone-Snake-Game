#pragma once

#include <memory>

#include "game_states.hpp"

class GameStateFactory
{
public:
    GameStateFactory(int grid_width, int grid_height)
        : grid_width_{grid_width}
        , grid_height_{grid_height}
    {}

    ~GameStateFactory() = default;

public:
    std::unique_ptr<GameStateBase> CreateGameState(GameStateType type) {
        switch (type) {
            case GameStateType::MAIN_MENU:
                return std::make_unique<MainMenuState>();
            case GameStateType::SINGLE_PLAYER_GAME:
                return std::make_unique<SinglePlayerGameState>(grid_width_, grid_height_);
            case GameStateType::PvP_GAME:
                return std::make_unique<PvPGameState>(grid_width_, grid_height_);
            case GameStateType::PAUSE_GAME:
                return std::make_unique<PauseGameState>();
            default:
                return nullptr;
        }
    }

private:
    int grid_width_;
    int grid_height_;

};
