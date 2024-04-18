#include "game_state/multiplayer_state.hpp"

#include "game_field/gameplay_manager.hpp"


MultiplayerGameState::MultiplayerGameState(const std::size_t screenWidth, const std::size_t screenHeight,
                                           const GameStateType stateType) :
	::GameState<MultiplayerGameScreen>(stateType, std::make_unique<MultiplayerGameScreen>(screenWidth, screenHeight))
{}

void MultiplayerGameState::Init() {
    GameState::Init();
}

void MultiplayerGameState::Update() {
    GameState::Update();
    screen_->Score(
        gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_1),
        gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_2)
    );
}

void MultiplayerGameState::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    GameState::Render(sdl_renderer, block);
    HandleScreenEvent(screen_->ScreenEvent());
}

void MultiplayerGameState::HandleScreenEvent(const MultiplayerGameEvent event) {
    switch (event) {
    case MultiplayerGameEvent::MAIN_MENU:
        nextStateType_ = GameStateType::MAIN_MENU;
        break;
    case MultiplayerGameEvent::RESTART_GAME:
        nextStateType_ = GameStateType::RESTART_GAME;
        break;
    case MultiplayerGameEvent::QUIT_GAME:
        nextStateType_ = GameStateType::QUIT_GAME;
        break;
    default:
        nextStateType_ = GameStateType::NONE;
        break;
    }
}

void MultiplayerGameState::HandleInput(SDL_Keycode keyCode) {
    GameState::HandleInput(keyCode);
    switch (keyCode)
    {
    case SDLK_p:
    case SDLK_ESCAPE:
        nextStateType_ = GameStateType::PAUSE_GAME;
        break;
    }
}

void MultiplayerGameState::HandleGameFieldStateChange(const GameFieldState state) {
    if (state == GameFieldState::GAME_OVER) {
        auto const bestScore = gameSaveData_->GetBestScore(StateType());

        const auto& winner = gamePlayManager_->gameField()->GetWinner();
        screen_->SetWinner(winner, bestScore);

        if (winner.score > bestScore) {
            gameSaveData_->SetBestScore(StateType(), winner.score);
        }
    }
}
