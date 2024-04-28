#include "game_state/game_state_factory.hpp"

#include "game_state/game_state_type.hpp"
#include "game_state/single_player_state.hpp"
#include "game_state/multiplayer_state.hpp"
#include "game_state/main_menu_state.hpp"
#include "game_state/pause_state.hpp"

GameStateFactory::GameStateFactory(const std::size_t screenWidth, const std::size_t screenHeight) : screenWidth_{ screenWidth }, screenHeight_{ screenHeight }
{}

std::unique_ptr<GameStateBase> GameStateFactory::CreateGameState(const GameStateType type)
{
    switch (type) {
    case GameStateType::MAIN_MENU:
        return std::make_unique<MainMenuState>(screenWidth_, screenHeight_);
    case GameStateType::SINGLE_PLAYER_GAME:
        return std::make_unique<SinglePlayerGameState>(screenWidth_, screenHeight_);
    case GameStateType::PvP_GAME:
        return std::make_unique<MultiplayerGameState>(screenWidth_, screenHeight_, GameStateType::PvP_GAME);
    case GameStateType::PvAI_GAME:
        return std::make_unique<MultiplayerGameState>(screenWidth_, screenHeight_, GameStateType::PvAI_GAME);
    case GameStateType::PAUSE_GAME:
        return std::make_unique<PauseGameState>(screenWidth_, screenHeight_);
    default:
        return nullptr;
    }
}
