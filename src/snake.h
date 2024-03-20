#ifndef SNAKE_H
#define SNAKE_H

#include <unordered_map>
#include <vector>

#include "SDL.h"

#include "player_id.hpp"
#include "cell_type.hpp"


class Snake {
public:
    enum class Direction { kUp, kDown, kLeft, kRight };

    Snake(const std::size_t grid_width, const std::size_t grid_height, PlayerId player_id)
        : grid_width{ grid_width },
        grid_height{ grid_height },
        player_id{ player_id }
    {
        if (player_id == PlayerId::PLAYER_1)
        {
            cell_type = CellType::SNAKE_P1;
            initial_position = { 20.0f, grid_height - 20.0f };
            input_mapping = {
                { SDLK_UP, Direction::kUp },
                { SDLK_DOWN, Direction::kDown },
                { SDLK_LEFT, Direction::kLeft },
                { SDLK_RIGHT, Direction::kRight }
            };
        }
        else if (player_id == PlayerId::PLAYER_2)
        {
            cell_type = CellType::SNAKE_P2;
            initial_position = { grid_width - 20.0f, grid_height - 20.0f };
            input_mapping = {
                { SDLK_w, Direction::kUp },
                { SDLK_s, Direction::kDown },
                { SDLK_a, Direction::kLeft },
                { SDLK_d, Direction::kRight }
            };
        }
    }

    bool Alive() const { return alive; }

    void Init();
    void Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field);

    bool GotFood(const SDL_Point& food) const;
    void GrowBody();
    void SpeedUp(const float value);
    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block);
    void HandleInput(SDL_Keycode key_code);

private:
    void UpdateHead();
    void UpdateBody(
        SDL_Point& current_head_cell,
        SDL_Point& prev_head_cell,
        std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field
    );

    bool growing{ false };
    int size{ 1 };
    std::size_t grid_width;
    std::size_t grid_height;

    std::vector<SDL_Point> body;
    bool alive{ false };
    float head_x{ 0.0f };
    float head_y{ 0.0f };
    float speed{ 0.1f };
    Direction direction{ Direction::kUp };

    PlayerId player_id;
    CellType cell_type;
    SDL_FPoint initial_position;
    std::unordered_map<SDL_Keycode, Direction> input_mapping;
};

#endif