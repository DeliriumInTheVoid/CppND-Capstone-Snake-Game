#include "game_field/gameplay_manager.hpp"


GamePlayManager::GamePlayManager(std::unique_ptr<GameFieldFactory>&& gameFieldFactory) :
    gameFieldFactory_{ std::move(gameFieldFactory) } {
    gameField_ = gameFieldFactory_->CreateGameField(stateType_);
}

const std::unique_ptr<GameField>& GamePlayManager::gameField() const {
    return gameField_;
}

void GamePlayManager::ChangeState(const GameStateType nextStateType) {
    if (nextStateType == GameStateType::PAUSE_GAME ||
        (stateType_ == GameStateType::PAUSE_GAME && nextStateType != GameStateType::MAIN_MENU))
    {
        stateType_ = nextStateType;
        return;
    }

    gameField_ = gameFieldFactory_->CreateGameField(nextStateType);
    gameField_->Init();
    stateType_ = nextStateType;
}

void GamePlayManager::HandleInput(const SDL_Keycode keyCode) const {
    gameField_->HandleInput(keyCode);
}

GameFieldState GamePlayManager::Update() const {
    return gameField_->Update();
}

void GamePlayManager::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) const {
    gameField_->Render(sdl_renderer, block);
}
