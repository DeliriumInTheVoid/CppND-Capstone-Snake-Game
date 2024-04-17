#pragma once

#include <memory>
#include "SDL.h"

#include "game_field.hpp"
#include "gamefield_factory.hpp"
#include "game_field_state.hpp"


enum class GameStateType: unsigned;

class GamePlayManager
{
public:
    GamePlayManager(std::unique_ptr<GameFieldFactory>&& gameFieldFactory):
        gameFieldFactory_{std::move(gameFieldFactory)} {
        gameField_ = gameFieldFactory_->createGameField(stateType_);
    }

    ~GamePlayManager() = default;

public:
    const std::unique_ptr<GameField>& gameField() const { return gameField_; }

public:
    void ChangeState(const GameStateType nextStateType) {
        if (nextStateType == GameStateType::PAUSE_GAME ||
            (stateType_ == GameStateType::PAUSE_GAME && nextStateType != GameStateType::MAIN_MENU))
        {
            stateType_ = nextStateType;
            return;
        }
    
        gameField_ = gameFieldFactory_->createGameField(nextStateType);
        gameField_->Init();
        stateType_ = nextStateType;
    }

    void HandleInput(const SDL_Keycode keyCode) const {
        gameField_->HandleInput(keyCode);
    }

    GameFieldState Update() const {
        return gameField_->Update();
    }

    void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) const {
        gameField_->Render(sdl_renderer, block);
    }
private:
    GameStateType stateType_{ GameStateType::NONE };
    std::unique_ptr<GameFieldFactory> gameFieldFactory_;
    std::unique_ptr<GameField> gameField_;
};
