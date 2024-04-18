#include "game_state/single_player_state.hpp"
#include "player/player_id.hpp"

SinglePlayerGameState::SinglePlayerGameState(const std::size_t screenWidth, const std::size_t screenHeight) :
    GameState(GameStateType::SINGLE_PLAYER_GAME, std::make_unique<SinglePlayerGameScreen>(screenWidth, screenHeight))
{}

void SinglePlayerGameState::Init() {
    GameState::Init();
}

void SinglePlayerGameState::Update() {
    GameState::Update();
    screen_->Score(gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_1));
}

void SinglePlayerGameState::HandleInput(const SDL_Keycode keyCode) {
    GameState::HandleInput(keyCode);
    switch (keyCode)
    {
    case SDLK_p:
    case SDLK_ESCAPE:
        nextStateType_ = GameStateType::PAUSE_GAME;
        break;
    }
}

void SinglePlayerGameState::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    GameState::Render(sdl_renderer, block);
    HandleScreenEvent(screen_->ScreenEvent());
}

void SinglePlayerGameState::HandleScreenEvent(const SinglePlayerGameEvent event) {
    switch (event) {
    case SinglePlayerGameEvent::MAIN_MENU:
        nextStateType_ = GameStateType::MAIN_MENU;
        break;
    case SinglePlayerGameEvent::RESTART_GAME:
        nextStateType_ = GameStateType::RESTART_GAME;
        break;
    case SinglePlayerGameEvent::QUIT_GAME:
        nextStateType_ = GameStateType::QUIT_GAME;
        break;
    default:
        nextStateType_ = GameStateType::NONE;
        break;
    }
}

void SinglePlayerGameState::HandleGameFieldStateChange(const GameFieldState state) {
    if (state == GameFieldState::GAME_OVER) {
        auto const bestScore = gameSaveData_->GetBestScore(StateType());

        screen_->GameOver(bestScore);

        auto const currentScore = gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_1);
        if (currentScore > bestScore) {
            gameSaveData_->SetBestScore(StateType(), currentScore);
        }
    }
}
