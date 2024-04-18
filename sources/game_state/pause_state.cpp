#include "game_state/pause_state.hpp"


PauseGameState::PauseGameState(const std::size_t screenWidth, const std::size_t screenHeight) :
    GameState(GameStateType::PAUSE_GAME, std::make_unique<PauseGameScreen>(screenWidth, screenHeight))
{}

void PauseGameState::Update() {
    nextStateType_ = GameStateType::NONE;
    screen_->Update();
}

void PauseGameState::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    GameState::Render(sdl_renderer, block);
    HandleScreenEvent(screen_->ScreenEvent());
}

void PauseGameState::HandleScreenEvent(const PauseGameEvent event) {
    switch (event) {
    case PauseGameEvent::RESUME_GAME:
        nextStateType_ = GameStateType::RESUME_GAME;
        break;
    case PauseGameEvent::QUIT_GAME:
        nextStateType_ = GameStateType::MAIN_MENU;
        break;
    default:
        nextStateType_ = GameStateType::NONE;
        break;
    }
}

void PauseGameState::HandleInput(const SDL_Keycode keyCode) {
    switch (keyCode)
    {
    case SDLK_p:
    case SDLK_ESCAPE:
        nextStateType_ = GameStateType::RESUME_GAME;
        break;
    }
}
