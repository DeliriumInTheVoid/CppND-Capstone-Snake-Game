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
    GameField(const std::size_t grid_width, const std::size_t grid_height)
        : width_{ grid_width }
        , height_{ grid_height }
    {}
    virtual ~GameField() {}

public:
    virtual void Init() = 0;
    virtual GameFieldState Update() = 0;
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) = 0;
    virtual void HandleInput(SDL_Keycode key_code) = 0;

    virtual int PlayerScore(PlayerId player_id) const = 0;

    virtual PlayerId Winner() const { return PlayerId::UNKNOWN; }

public:
    GameFieldState GameState() const { return gameFieldState_; }

    void AddPlayer(std::unique_ptr<Player>&& player)
    {
        players_.emplace(player->Id(), std::move(player));
    }

protected:
    GameFieldState gameFieldState_{ GameFieldState::NONE };
    std::unordered_map<PlayerId, std::unique_ptr<Player>> players_{};

private:
    std::size_t width_;
    std::size_t height_;
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
    PlayerGameField(std::size_t grid_width, std::size_t grid_height)
        : GameField(grid_width, grid_height),
        randomEngine_(randomDev_()),
        randomWidth_(0, static_cast<int>(grid_width - 1)),
        randomHeight_(0, static_cast<int>(grid_height - 1))
    {}

    ~PlayerGameField() = default;
public:
    void Init() override {
        for (const auto& [id, player] : players_) {
            player->Init();
        }

        for (std::size_t i = 0; i <= 32; ++i) {
            fieldCells_.emplace(i, std::unordered_map<std::size_t, CellType>{});
            for (std::size_t j = 0; j <= 32; ++j) {
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
            player->Update(&fieldCells_);
            Snake& snake = player->GetSnake();

            if (!snake.Alive()) {
                gameFieldState_ = GameFieldState::GAME_OVER;
                return gameFieldState_;
            }

            if (snake.GotFood(food_)) {
                player->AddScore(1);
                PlaceFood();
                snake.GrowBody();
                snake.SpeedUp(0.02f);
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

    virtual PlayerId Winner() const override {
        PlayerId winner{ PlayerId::UNKNOWN };

        if (gameFieldState_ != GameFieldState::GAME_OVER) {
            return winner;
        }

        for (const auto& [id, player] : players_) {
            Snake& snake = player->GetSnake();

            if (snake.Alive()) {
                winner = player->Id();
                break;
            }
        }

        return winner;
    }

private:

    void PlaceFood() {
        while (true)
        {
            const int x = randomWidth_(randomEngine_);
            const int y = randomHeight_(randomEngine_);
            // Check that the location is not occupied by a snake item before placing
            // food.
            if (fieldCells_[y][x] == CellType::EMPTY) {
                food_.x = x;
                food_.y = y;
                fieldCells_[y][x] = CellType::FOOD;
                return;
            }
        }
    }

private:
    //Snake snake;
    SDL_Point food_{};
    //int score;
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> fieldCells_{};

    std::random_device randomDev_;
    std::mt19937 randomEngine_;
    std::uniform_int_distribution<int> randomWidth_;
    std::uniform_int_distribution<int> randomHeight_;
};
