#ifndef SNAKE_H
#define SNAKE_H

#include <unordered_map>
#include <vector>

#include "SDL.h"

#include "player_id.hpp"
#include "cell_type.hpp"
#include "move_direction.hpp"


class Snake {
public:
    Snake(const std::size_t gridWidth, const std::size_t gridHeight, PlayerId playerId)
        : gridWidth_{ gridWidth },
        gridHeight_{ gridHeight }
    {
        if (playerId == PlayerId::PLAYER_1)
        {
            cellType_ = CellType::SNAKE_P1;
            initialPosition_ = { 5.0f, static_cast<float>(gridHeight) - 5.0f };
        }
        else if (playerId == PlayerId::PLAYER_2)
        {
            cellType_ = CellType::SNAKE_P2;
            initialPosition_ = {
                static_cast<float>(gridWidth) - 5.0f,
                static_cast<float>(gridHeight) - 5.0f
            };
        }
    }

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
