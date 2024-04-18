#include "game_state/game_state_manager.hpp"
#include "game_field/gameplay_manager.hpp"


GameStateManager::GameStateManager(std::unique_ptr<GameStateFactory> gameStateFactory,
    std::unique_ptr<GamePlayManager>&& gamePlayManager, std::unique_ptr<GameSaveData>&& gameSaveData) : gameStateFactory_{ std::move(gameStateFactory) },
    currentStateType_{ GameStateType::NONE } {

    gameStates_[GameStateType::MAIN_MENU] = gameStateFactory_->CreateGameState(GameStateType::MAIN_MENU);
    const auto& currentState = gameStates_[GameStateType::MAIN_MENU];
    currentState->MoveGamePlayManager(std::move(gamePlayManager));
    gameSaveData->PreloadData();
    currentState->MoveGameSaveData(std::move(gameSaveData));
    currentState->Init();
    currentStateType_ = GameStateType::MAIN_MENU;
}

GameStateType GameStateManager::CurrentStateType() const {
    return currentStateType_;
}

void GameStateManager::HandleInput(const SDL_Keycode key_code) {
    gameStates_[currentStateType_]->HandleInput(key_code);
    CheckStateChange();
}

void GameStateManager::Update() {
    gameStates_[currentStateType_]->Update();
    CheckStateChange();
}

void GameStateManager::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    const auto& currentState = gameStates_[currentStateType_];
    currentState->Render(sdl_renderer, block);
    CheckStateChange();
}

void GameStateManager::CheckStateChange() {
    const auto& currentState = gameStates_[currentStateType_];
    if (const auto nextStateType = currentState->NextStateType(); nextStateType != GameStateType::NONE) {
        ChangeState(nextStateType);
    }
}

void GameStateManager::ChangeState(GameStateType nextStateType) {
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

    gameStates_[nextStateType]->MoveGameSaveData(
        std::move(gameStates_[currentStateType_]->MoveGameSaveData())
    );

    currentStateType_ = nextStateType;
    gameStates_[currentStateType_]->Init();
}
