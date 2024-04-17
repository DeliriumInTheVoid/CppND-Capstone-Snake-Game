#pragma once

#include "snake.h"
#include "player_id.hpp"
#include "cell_type.hpp"
#include "move_direction.hpp"
#include "a_star_async.hpp"


struct Winner {
    Winner() : Winner(PlayerId::UNKNOWN, "NoName", 0)
    {}

    Winner(const PlayerId id, const std::string_view name, const int score) :
        id(id), name{ name }, score(score)
    {}

    PlayerId id;
    std::string_view name;
    int score;
};

class Player
{
public:
    Player(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight) :
        snake_{ gridWidth, gridHeight, id }, gridHeight_{gridHeight}, gridWidth_{ gridWidth }, id_{id}
    {
        if (id == PlayerId::PLAYER_1) {
            inputMapping_ = {
                {SDLK_UP, Direction::kUp},
                {SDLK_DOWN, Direction::kDown},
                {SDLK_LEFT, Direction::kLeft},
                {SDLK_RIGHT, Direction::kRight}
            };
            name_ = "Player 1";
        }
        else if (id == PlayerId::PLAYER_2)
        {
            inputMapping_ = {
                { SDLK_w, Direction::kUp },
                { SDLK_s, Direction::kDown },
                { SDLK_a, Direction::kLeft },
                { SDLK_d, Direction::kRight }
            };
            name_ = "Player 2";
        }
    }

    virtual ~Player() = default;

public:
    void Init() {

        if (snake_.Alive()) {
            return;
        }
        snake_.Init();
        score_ = 0;
    }

    void Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
        if (!snake_.Alive()) {
            return;
        }
        
        UpdateSnake(field);
    }

    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
        snake_.Render(sdl_renderer, block);
    }

    virtual void HandleInput(const SDL_Keycode keyCode) {
        if (const auto& it = inputMapping_.find(keyCode); it != inputMapping_.end()) {
            snake_.MoveTo(it->second);
        }
    }

    void AddScore(const int value) {
        score_ += value;
    }

    int Score() const { return score_; }
    PlayerId Id() const { return id_; }
    std::string_view Name() const { return name_; }
    Snake& GetSnake() { return snake_; }

protected:
    virtual bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
        return snake_.Update(field);
    }

protected:
    std::string_view name_;
    std::unordered_map<SDL_Keycode, Direction> inputMapping_;
    Snake snake_;
    std::size_t gridHeight_;
    std::size_t gridWidth_;

private:
    PlayerId id_;
    int score_{ 0 };
    int hunger_{ 0 };
};


class PlayerAI : public Player
{
public:
    PlayerAI(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight) :
        Player{ id, gridWidth, gridHeight }, aStar_{ gridWidth, gridHeight }
    {
        name_ = "AI Player";
    }

    virtual ~PlayerAI() override = default;

public:
    //void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {
    //    Player::Render(sdl_renderer, block);

    //    if (!path_.empty()) {
    //        for (auto it = path_.begin() + 1; it != path_.end(); ++it) {
    //            const auto* cell = *it;
    //            SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);
    //            block.x = cell->x * block.w;
    //            block.y = cell->y * block.h;
    //            SDL_RenderFillRect(sdl_renderer, &block);
    //        }
    //    }
    //}

    virtual void HandleInput(const SDL_Keycode key_code) override
    {
    }

protected:
    bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) override {

        if (aStar_.HasResult()) {
            //field->at(cell->x).at(cell->y) = CellType::MOVE_GOAL;
            const auto cell = aStar_.GetResult();

            if (!cell.isValid) {
	            return false;
            }

            if (cell.x > snake_.HeadX() && snake_.MoveDirection() != Direction::kLeft) {
                snake_.MoveTo(Direction::kRight);
            }
            else if (cell.x < snake_.HeadX() && snake_.MoveDirection() != Direction::kRight) {
                snake_.MoveTo(Direction::kLeft);
            }
            else if (cell.y > snake_.HeadY() && snake_.MoveDirection() != Direction::kUp) {
                snake_.MoveTo(Direction::kDown);
            }
            else if (cell.y < snake_.HeadY() && snake_.MoveDirection() != Direction::kDown) {
                snake_.MoveTo(Direction::kUp);
            }
        }

        if (!Player::UpdateSnake(field)) {
            return false;
        }

        if (aStar_.ReadyToSearch()) {
            aStar_.findPathAsync(snake_.HeadX(), snake_.HeadY(), CellType::FOOD, field);
        }

        return true;
    }

private:
    ResultCell nextMoveCell{};
    AStartAsync aStar_;
};
