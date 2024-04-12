#pragma once

#include <memory>
#include <map>
#include "game_states.hpp"
#include "game_state_factory.hpp"
#include "gameplay_manager.hpp"


class GameStateManager
{
public:
    GameStateManager(std::unique_ptr<GameStateFactory> gameStateFactory, std::unique_ptr<GamePlayManager> gamePlayManager)
        : gameStateFactory_{std::move(gameStateFactory)},
          currentStateType_{GameStateType::NONE},
          gamePlayManager_{std::move(gamePlayManager)} {
        gameStates_[GameStateType::MAIN_MENU] = gameStateFactory_->CreateGameState(GameStateType::MAIN_MENU);
        const auto& currentState = gameStates_[GameStateType::MAIN_MENU];
        currentState->MoveGamePlayManager(std::move(gamePlayManager_));
        currentState->Init();
        currentStateType_ = GameStateType::MAIN_MENU;
    }

    ~GameStateManager() = default;

public:
    GameStateType CurrentStateType() const {
        return currentStateType_;
    }

    void HandleInput(const SDL_Keycode key_code) {
        gameStates_[currentStateType_]->HandleInput(key_code);
        CheckStateChange();
    }

    void Update() {
        gameStates_[currentStateType_]->Update();
        CheckStateChange();
    }

    void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) {
        const auto& currentState = gameStates_[currentStateType_];
        currentState->Render(sdl_renderer, block);
        CheckStateChange();
    }

private:
    void CheckStateChange() {
        const auto& currentState = gameStates_[currentStateType_];
        if (const auto nextStateType = currentState->NextStateType(); nextStateType != GameStateType::NONE) {
            ChangeState(nextStateType);
        }
    }

    void ChangeState(GameStateType nextStateType) {
        if (nextStateType == GameStateType::NONE) {
            return;
        }

        if (nextStateType == GameStateType::QUIT_GAME) {
            currentStateType_ = GameStateType::QUIT_GAME;
            return;
        }

        if (nextStateType == GameStateType::PAUSE_GAME) {
            pausedStateType_ = currentStateType_;
        }
        else if (nextStateType == GameStateType::RESUME_GAME) {
            nextStateType = pausedStateType_;
            pausedStateType_ = GameStateType::NONE;
        }
        else if (nextStateType == GameStateType::RESTART_GAME) {
            nextStateType = currentStateType_;
        }

        if (gameStates_.find(nextStateType) == gameStates_.end()) {
            gameStates_[nextStateType] = gameStateFactory_->CreateGameState(nextStateType);
        }

        gameStates_[nextStateType]->MoveGamePlayManager(
            std::move(gameStates_[currentStateType_]->MoveGamePlayManager())
        );
        currentStateType_ = nextStateType;
        gameStates_[currentStateType_]->Init();
    }

private:
    std::unique_ptr<GameStateFactory> gameStateFactory_;
    GameStateType currentStateType_;
    GameStateType pausedStateType_{ GameStateType::NONE };
    std::map<GameStateType, std::unique_ptr<GameStateBase>> gameStates_;
    std::unique_ptr<GamePlayManager> gamePlayManager_;
};
