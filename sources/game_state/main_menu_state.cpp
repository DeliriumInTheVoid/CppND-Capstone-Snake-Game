#include "game_state/main_menu_state.hpp"

#include "game_field/gameplay_manager.hpp"
#include "game_state/game_states.hpp"
#include "screen/main_menu_screen.hpp"

MainMenuState::MainMenuState(const std::size_t screenWidth, const std::size_t screenHeight) :
    GameState(GameStateType::MAIN_MENU, std::make_unique<MainMenuScreen>(screenWidth, screenHeight))
{}

void MainMenuState::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    GameState::Render(sdl_renderer, block);
    HandleScreenEvent(screen_->ScreenEvent());
}

void MainMenuState::HandleScreenEvent(const MainMenuEvent event) {
    switch (event)
    {
    case MainMenuEvent::SINGLE_PLAYER_GAME:
        nextStateType_ = GameStateType::SINGLE_PLAYER_GAME;
        break;
    case MainMenuEvent::PvP_GAME:
        nextStateType_ = GameStateType::PvP_GAME;
        break;
    case MainMenuEvent::PvAI_GAME:
        nextStateType_ = GameStateType::PvAI_GAME;
        break;
    case MainMenuEvent::QUIT_GAME:
        nextStateType_ = GameStateType::QUIT_GAME;
        break;
    default:
        nextStateType_ = GameStateType::NONE;
        break;
    }
}
