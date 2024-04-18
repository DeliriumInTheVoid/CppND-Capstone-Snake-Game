#include "game_state/game_states.hpp"


GameStateBase::GameStateBase(const GameStateType stateType) : gameStateType_{ stateType }
{}

GameStateType GameStateBase::StateType() const {
	return gameStateType_;
}

GameStateType GameStateBase::NextStateType() const {
	return nextStateType_;
}

void GameStateBase::MoveGamePlayManager(std::unique_ptr<GamePlayManager>&& manager) {
	gamePlayManager_ = std::move(manager);
	gamePlayManager_->ChangeState(gameStateType_);
}

std::unique_ptr<GamePlayManager>&& GameStateBase::MoveGamePlayManager() {
	return std::move(gamePlayManager_);
}

void GameStateBase::MoveGameSaveData(std::unique_ptr<GameSaveData>&& data) {
	gameSaveData_ = std::move(data);
}

std::unique_ptr<GameSaveData>&& GameStateBase::MoveGameSaveData() {
	return std::move(gameSaveData_);
}

void GameStateBase::Update() {
	nextStateType_ = GameStateType::NONE;
	if (const auto state = gamePlayManager_->Update(); state != gmeFieldState_) {
		gmeFieldState_ = state;
		HandleGameFieldStateChange(gmeFieldState_);
	}
}

void GameStateBase::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
	gamePlayManager_->Render(sdl_renderer, block);
}

void GameStateBase::Init() {
	nextStateType_ = GameStateType::NONE;
}

void GameStateBase::HandleInput(SDL_Keycode keyCode) {
	gamePlayManager_->HandleInput(keyCode);
}
