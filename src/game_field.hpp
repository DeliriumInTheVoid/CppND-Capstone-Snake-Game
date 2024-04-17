#pragma once

#include <unordered_map>

#include "SDL.h"

#include "player.hpp"
#include "snake.h"
#include "cell_type.hpp"
#include "game_field_state.hpp"

class GameField
{
public:
    GameField(const std::size_t gridWidth, const std::size_t gridHeight)
        : gridWidth_{ gridWidth }
        , gridHeight_{ gridHeight }
    {}
    virtual ~GameField() {}

public:
    virtual void Init() = 0;
    virtual GameFieldState Update() = 0;
    virtual void Render(SDL_Renderer* sdlRenderer, SDL_Rect& block) = 0;
    virtual void HandleInput(SDL_Keycode keyCode) = 0;

    virtual int PlayerScore(PlayerId player_id) const = 0;

    virtual Winner GetWinner() const { return {}; }

public:
    GameFieldState GameState() const { return gameFieldState_; }

    void AddPlayer(std::unique_ptr<Player>&& player)
    {
        players_.emplace(player->Id(), std::move(player));
    }

protected:
    std::size_t gridWidth_;
    std::size_t gridHeight_;

    GameFieldState gameFieldState_{ GameFieldState::NONE };
    std::unordered_map<PlayerId, std::unique_ptr<Player>> players_{};
};

class NullGameField : public GameField
{
public:
    NullGameField()
        : GameField(0, 0)
    {}

    virtual ~NullGameField() override {}

public:
    void Init() override {}
    GameFieldState Update() override { return GameFieldState::NONE; }
    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {}
    void HandleInput(SDL_Keycode keyCode) override {}
    int PlayerScore(PlayerId PlayerId) const override { return 0; }
};


class PlayerGameField : public GameField
{
public:
    PlayerGameField(std::size_t gridHidth, std::size_t gridHeight)
        : GameField(gridHidth, gridHeight),
        randomEngine_(randomDev_()),
        randomWidth_(0, static_cast<int>(gridHidth - 1)),
        randomHeight_(0, static_cast<int>(gridHeight - 1))
    {}

    ~PlayerGameField() override = default;
public:
    void Init() override {
        for (const auto& [id, player] : players_) {
            player->Init();
        }

        for (std::size_t i = 0; i <= gridWidth_; ++i) {
            fieldCells_.emplace(i, std::unordered_map<std::size_t, CellType>{});
            for (std::size_t j = 0; j <= gridHeight_; ++j) {
                auto& row = fieldCells_[i];
                row.emplace(j, CellType::EMPTY);
            }
        }

        PlaceFood();

        gameFieldState_ = GameFieldState::GAME_PLAY;
    }

    GameFieldState Update() override {

        if (gameFieldState_ != GameFieldState::GAME_PLAY) {
            return gameFieldState_;
        }

        for (const auto& [id, player] : players_) {
            player->Update(fieldCells_);
            Snake& snake = player->GetSnake();

            if (!snake.Alive()) {
                gameFieldState_ = GameFieldState::GAME_OVER;
                return gameFieldState_;
            }

            if (snake.GotFood(food_)) {
                fieldCells_[food_.x][food_.y] = CellType::EMPTY;
                player->AddScore(1);
                snake.GrowBody();
                snake.SpeedUp(0.01f);
                PlaceFood();
            }
        }

        return gameFieldState_;
    }

    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {
        //TODO:: Draw field_cells according to their type.
        //Remove render from other game_field entities (snake, food, obstacle etc.) only update should to left.
        for (const auto& [id, player] : players_) {
            player->Render(sdl_renderer, block);
        }
        // Render food
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
        block.x = food_.x * block.w;
        block.y = food_.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    void HandleInput(const SDL_Keycode key_code) override {
        for (const auto& [id, player] : players_) {
            player->HandleInput(key_code);
        }
    }

    int PlayerScore(const PlayerId player_id) const override {
        return players_.find(player_id)->second->Score();
    }

    virtual Winner GetWinner() const override {

        if (gameFieldState_ != GameFieldState::GAME_OVER) {
            return {};
        }

        for (const auto& [id, player] : players_) {
            Snake& snake = player->GetSnake();

            if (snake.Alive()) {
                return { player->Id(), player->Name(), player->Score() };
            }
        }

        return {};
    }

private:

    void PlaceFood() {
        while (true)
        {
            const int x = randomWidth_(randomEngine_);
            const int y = randomHeight_(randomEngine_);
            // Check that the location is not occupied by a snake item before placing
            // food.
            if (fieldCells_[x][y] == CellType::EMPTY) {
                food_.x = x;
                food_.y = y;
                fieldCells_[x][y] = CellType::FOOD;
                return;
            }
        }
    }

private:
    SDL_Point food_{};
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> fieldCells_{};

    std::random_device randomDev_;
    std::mt19937 randomEngine_;
    std::uniform_int_distribution<int> randomWidth_;
    std::uniform_int_distribution<int> randomHeight_;
};
