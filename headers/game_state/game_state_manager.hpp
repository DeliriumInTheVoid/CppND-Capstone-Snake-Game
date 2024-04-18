#pragma once

#include <memory>
#include <map>

#include "SDL_keycode.h"
#include "SDL_render.h"

#include "game_state/game_states.hpp"
#include "game_state/game_state_type.hpp"
#include "game_state/game_state_factory.hpp"

class GamePlayManager;
class GameSaveData;

class GameStateManager
{
public:
    GameStateManager(std::unique_ptr<GameStateFactory> gameStateFactory,
        std::unique_ptr<GamePlayManager>&& gamePlayManager,
        std::unique_ptr<GameSaveData>&& gameSaveData);

    ~GameStateManager() = default;

public:
    GameStateType CurrentStateType() const;
    void HandleInput(const SDL_Keycode key_code);
    void Update();
    void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block);

private:
    void CheckStateChange();
    void ChangeState(GameStateType nextStateType);

private:
    std::unique_ptr<GameStateFactory> gameStateFactory_;
    GameStateType currentStateType_;
    GameStateType pausedStateType_{ GameStateType::NONE };
    std::map<GameStateType, std::unique_ptr<GameStateBase>> gameStates_;
};
