#pragma once

#include "snake.h"
#include "player_id.hpp"
#include "cell_type.hpp"


class Player
{
public:
    Player(const PlayerId id, const std::size_t grid_width, const std::size_t grid_height) :
        id{ id }, snake{ grid_width, grid_height, id }
    {}

    ~Player() = default;

public:
    void Init() {

        if (snake.Alive()) {
            return;
        }
        snake.Init();
        score = 0;
    }

    void Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field) {
        if (!snake.Alive()) {
            return;
        }

        snake.Update(field);
    }

    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
        snake.Render(sdl_renderer, block);
    }

    void HandleInput(const SDL_Keycode key_code) {
        snake.HandleInput(key_code);
    }

    void AddScore(const int value) {
        score += value;
    }

    int Score() const { return score; }
    PlayerId Id() const { return id; }
    Snake& GetSnake() { return snake; }

private:
    PlayerId id;
    Snake snake;
    int score{ 0 };
    int hunger{ 0 };
};
