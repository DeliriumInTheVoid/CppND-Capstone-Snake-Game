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
    GamePlayManager(std::unique_ptr<GameFieldFactory> gameFieldFactory/*, std::size_t grid_width, std::size_t grid_height*/):
        gameFieldFactory_{std::move(gameFieldFactory)}
        // grid_width_{grid_width}, 
        // grid_height_{grid_height}
    {
        gameField_ = gameFieldFactory_->createGameField(stateType_);
    }
    ~GamePlayManager() {}

public:
    const std::unique_ptr<GameField>& gameField() const { return gameField_; }

public:
    void ChangeState(GameStateType nextStateType)
    {
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

    void HandleInput(SDL_Keycode keyCode) const
    {
        gameField_->HandleInput(keyCode);
    }

    GameFieldState Update()
    {
        return gameField_->Update();
    }

    void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) const
    {
        gameField_->Render(sdl_renderer, block);
    }
private:
    GameStateType stateType_{ GameStateType::NONE };
    std::unique_ptr<GameFieldFactory> gameFieldFactory_;
    std::unique_ptr<GameField> gameField_;
    // std::size_t grid_width_;
    // std::size_t grid_height_;
};