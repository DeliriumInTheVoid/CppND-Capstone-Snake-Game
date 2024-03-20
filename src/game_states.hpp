#pragma once

#include <random>

#include "SDL_keycode.h"

#include "game_state_type.hpp"
#include "gameplay_manager.hpp"
#include "screen.hpp"
#include "snake.h"
#include "game_field_state.hpp"


class GameStateBase
{
public:
    GameStateBase(GameStateType stateType): gameStateType_{stateType}
    {}
    virtual ~GameStateBase() = default;

    // getters/setters
public:
    GameStateType StateType() const { return gameStateType_; }
    GameStateType NextStateType() const { return nextStateType_; }

// class methods
public:
    void MoveGamePlayManager(std::unique_ptr<GamePlayManager>&& manager)
    {
        gamePlayManager_ = std::move(manager);
        gamePlayManager_->ChangeState(gameStateType_);
    }

    std::unique_ptr<GamePlayManager>&& MoveGamePlayManager()
    {
        return std::move(gamePlayManager_);
    }

    virtual void Update()
    {
        nextStateType_ = GameStateType::NONE;
        if (const auto state = gamePlayManager_->Update(); state != gmeFieldState_) {
            gmeFieldState_ = state;
            HandleGameFieldStateChange(gmeFieldState_);
        }
    }

    virtual void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) 
    {
        gamePlayManager_->Render(sdl_renderer, block);
    }

    virtual void Init()
    {
        nextStateType_ = GameStateType::NONE;
    }

    virtual void HandleInput(SDL_Keycode keyCode) {
        gamePlayManager_->HandleInput(keyCode);
    }

protected:
    std::unique_ptr<GamePlayManager> gamePlayManager_;
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
// constructors/destructors
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


class MainMenuState : public GameState<MainMenuScreen>
{
public:
    MainMenuState(): GameState(GameStateType::MAIN_MENU, std::make_unique<MainMenuScreen>())
    {}

    virtual void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) override
    {
        GameState::Render(sdl_renderer, block);
        handleScreenEvent(screen_->ScreenEvent());
    }

    void handleScreenEvent(MainMenuEvent event)
    {
        switch (event)
        {
            case MainMenuEvent::SINGLE_PLAYER_GAME:
                nextStateType_ = GameStateType::SINGLE_PLAYER_GAME;
                break;
            case MainMenuEvent::PvP_GAME:
                nextStateType_ = GameStateType::PvP_GAME;
                break;
            case MainMenuEvent::QUIT_GAME:
                nextStateType_ = GameStateType::QUIT_GAME;
                break;
            default:
                nextStateType_ = GameStateType::NONE;
                break;
        }
    }
};

class SinglePlayerGameState : public GameState<SinglePlayerGameScreen>
{
public:
    SinglePlayerGameState(int grid_width, int grid_height) :
        GameState(GameStateType::SINGLE_PLAYER_GAME, std::make_unique<SinglePlayerGameScreen>())
    {}

    virtual ~SinglePlayerGameState() override = default;

public:
    virtual void Init() override {
        GameState::Init();
    }

    virtual void Update() override {
        GameState::Update();
        screen_->Score(gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_1));
    }

    virtual void HandleInput(SDL_Keycode keyCode) override {
        GameState::HandleInput(keyCode);
        switch (keyCode)
        {
            case SDLK_p:
            case SDLK_ESCAPE:
                nextStateType_ = GameStateType::PAUSE_GAME;
                break;
        }
    }

    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {
        GameState::Render(sdl_renderer, block);
        handleScreenEvent(screen_->ScreenEvent());
    }

    void handleScreenEvent(const SinglePlayerGameEvent event) {
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

protected:
    void HandleGameFieldStateChange(const GameFieldState state) override {
        if (state == GameFieldState::GAME_OVER) {
            screen_->GameOver();
        }
    }
};

class PvPGameState : public GameState<PvPGameScreen>
{
public:
    PvPGameState(int grid_width, int grid_height) :
        GameState(GameStateType::PvP_GAME, std::make_unique<PvPGameScreen>())
    {}

public:
    virtual void Init() override {
        GameState::Init();
    }

    virtual void Update() override {
        GameState::Update();
        screen_->Score(
            gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_1),
            gamePlayManager_->gameField()->PlayerScore(PlayerId::PLAYER_2)
        );
    }

    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {
        GameState::Render(sdl_renderer, block);
        handleScreenEvent(screen_->ScreenEvent());
    }

    void handleScreenEvent(const MultiplayerGameEvent event) {
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

    virtual void HandleInput(SDL_Keycode keyCode) override {
        GameState::HandleInput(keyCode);
        switch (keyCode)
        {
        case SDLK_p:
        case SDLK_ESCAPE:
            nextStateType_ = GameStateType::PAUSE_GAME;
            break;
        }
    }

    void HandleGameFieldStateChange(const GameFieldState state) override {
        if (state == GameFieldState::GAME_OVER) {
            screen_->Winner(gamePlayManager_->gameField()->Winner());
        }
    }
};

class PauseGameState : public GameState<PauseGameScreen>
{
public:
    PauseGameState(): GameState(GameStateType::PAUSE_GAME, std::make_unique<PauseGameScreen>())
    {}
    virtual ~PauseGameState() override 
    {}

public:
    virtual void Update() override {
        nextStateType_ = GameStateType::NONE;
        screen_->Update();
    }

    virtual void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block) override {
        GameState::Render(sdl_renderer, block);
        handleScreenEvent(screen_->ScreenEvent());
    }

    void handleScreenEvent(const PauseGameEvent event) {
        switch (event) {
            case PauseGameEvent::RESUME_GAME:
                nextStateType_ = GameStateType::SINGLE_PLAYER_GAME;
                break;
            case PauseGameEvent::QUIT_GAME:
                nextStateType_ = GameStateType::MAIN_MENU;
                break;
            default:
                nextStateType_ = GameStateType::NONE;
                break;
        }
    }

    virtual void HandleInput(const SDL_Keycode keyCode) override {
        switch (keyCode)
        {
            case SDLK_p:
                nextStateType_ = GameStateType::SINGLE_PLAYER_GAME;
                break;
        }
    }

};
