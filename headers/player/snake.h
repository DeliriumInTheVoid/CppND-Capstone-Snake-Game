#ifndef SNAKE_H
#define SNAKE_H

#include <unordered_map>
#include <vector>

#include "SDL.h"

#include "player/player_id.hpp"
#include "player/move_direction.hpp"
#include "game_field/cell_type.hpp"


class Snake {
public:
    Snake(const std::size_t gridWidth, const std::size_t gridHeight, PlayerId playerId);

    bool Alive() const { return alive; }
    int HeadX() const;
    int HeadY() const;
    Direction MoveDirection() const;

    void Init();
    bool Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field);

    bool GotFood(const SDL_Point& food) const;
    void GrowBody();
    void SpeedUp(const float value);
    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) const;
    void MoveTo(const Direction direction);

private:
    void UpdateHead();
    void UpdateBody(
        const SDL_Point& currentHeadCell,
        const SDL_Point& prevHeadCell,
        std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field
    );

    bool growing{ false };
    int size{ 1 };
    std::size_t gridWidth_;
    std::size_t gridHeight_;

    std::vector<SDL_Point> body;
    bool alive{ false };
    float headX_{ 0.0f };
    float headY_{ 0.0f };

private:
    float speed{ 0.1f };
    Direction direction_{ Direction::kUp };

private:
    CellType cellType_;
    SDL_FPoint initialPosition_;
};

#endif
