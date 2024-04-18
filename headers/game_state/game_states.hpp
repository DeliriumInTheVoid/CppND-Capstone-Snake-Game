#pragma once

#include <random>

#include "SDL_keycode.h"

#include "game_state/game_state_type.hpp"
#include "game_field/gameplay_manager.hpp"
#include "game_field/game_field_state.hpp"
#include "game_save_data.hpp"


class GameStateBase
{
public:
    GameStateBase(const GameStateType stateType);
    virtual ~GameStateBase() = default;

public:
    GameStateType StateType() const;
    GameStateType NextStateType() const;

public:
    void MoveGamePlayManager(std::unique_ptr<GamePlayManager>&& manager);
    std::unique_ptr<GamePlayManager>&& MoveGamePlayManager();
    void MoveGameSaveData(std::unique_ptr<GameSaveData>&& data);
    std::unique_ptr<GameSaveData>&& MoveGameSaveData();
    virtual void Update();
    virtual void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block);
    virtual void Init();
    virtual void HandleInput(SDL_Keycode keyCode);

protected:
    std::unique_ptr<GameSaveData> gameSaveData_{};
    std::unique_ptr<GamePlayManager> gamePlayManager_{};
    GameStateType nextStateType_{ GameStateType::NONE };

protected:
    virtual void HandleGameFieldStateChange(const GameFieldState state) {}

private:
    GameStateType gameStateType_;
    GameFieldState gmeFieldState_{ GameFieldState::NONE };
};


template <typename TScreen>
class GameState: public GameStateBase
{

public:
    GameState(const GameStateType stateType, std::unique_ptr<TScreen>&& screen): GameStateBase(stateType), screen_{std::move(screen)}
    {}

    virtual ~GameState() override = default;

public:
    virtual void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) override
    {
        GameStateBase::Render(sdl_renderer, block);
        screen_->Render();
    }

    virtual void Init() override
    {
        GameStateBase::Init();
        screen_->Init();
    }

    virtual void Update() override
    {
        GameStateBase::Update();
        screen_->Update();
    }

protected:
    std::unique_ptr<TScreen> screen_;
};
