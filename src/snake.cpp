#include <cmath>
#include "snake.h"
#include "move_direction.hpp"

int Snake::HeadX() const {
    return static_cast<int>(headX_);
}

int Snake::HeadY() const {
    return static_cast<int>(headY_);
}

Direction Snake::MoveDirection() const {
    return direction_;
}

void Snake::Init() {
    alive = true;
    size = 1;
    speed = 0.1f;
    headX_ = initialPosition_.x;
    headY_ = initialPosition_.y;
    body.clear();
    direction_ = Direction::kUp;
}

bool Snake::Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {

    if (!alive) {
        return false;
    }

    SDL_Point prev_cell{
        static_cast<int>(headX_),
        static_cast<int>(headY_) };  // We first capture the head's cell before updating.
    UpdateHead();
    SDL_Point current_cell{
        static_cast<int>(headX_),
        static_cast<int>(headY_) };  // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell, field);
        return true;
    }

    return false;
}

bool Snake::GotFood(const SDL_Point& food) const
{
    const int new_x = static_cast<int>(headX_);
    const int new_y = static_cast<int>(headY_);

    // Check if the snake head has reached the food
    if (new_x == food.x && new_y == food.y) {
        return true;
    }

    return false;
}

void Snake::UpdateHead() {
    switch (direction_) {
    case Direction::kUp:
        headY_ -= speed;
        break;

    case Direction::kDown:
        headY_ += speed;
        break;

    case Direction::kLeft:
        headX_ -= speed;
        break;

    case Direction::kRight:
        headX_ += speed;
        break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    headX_ = static_cast<float>(fmod(headX_ + static_cast<float>(gridWidth_), gridWidth_));
    headY_ = static_cast<float>(fmod(headY_ + static_cast<float>(gridHeight_), gridHeight_));
}

void Snake::UpdateBody(const SDL_Point& currentHeadCell, const SDL_Point& prevHeadCell,
                       std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
    // Add previous head location to vector
    body.push_back(prevHeadCell);

    if (!growing) {
        const auto& tail = body[0];
        field.at(tail.x).at(tail.y) = CellType::EMPTY;
        // Remove the tail from the vector.
        body.erase(body.begin());
    }
    else {
        growing = false;
        size++;
    }

    // Check if the snake has died.
    for (auto const& item : body) {
        field.at(item.x).at(item.y) = cellType_;
    }

    if (const auto cell_for_head = field.at(currentHeadCell.x).at(currentHeadCell.y);
        cell_for_head == CellType::SNAKE_P1 || cell_for_head == CellType::SNAKE_P2) {
        alive = false;
    }
    else {
        field.at(currentHeadCell.x).at(currentHeadCell.y) = cellType_;
    }
}

void Snake::GrowBody() { growing = true; }

void Snake::SpeedUp(const float value)
{
    if (speed >= 0.2f) {
        return;
    }

    speed += value;
}

void Snake::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) const {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const& point : body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(headX_) * block.w;
    block.y = static_cast<int>(headY_) * block.h;
    if (alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Snake::MoveTo(const Direction direction)
{
    switch (direction)
    {
    case Direction::kUp:
        if (direction_ != Direction::kDown) {
            direction_ = Direction::kUp;
        }
        break;
    case Direction::kDown:
        if (direction_ != Direction::kUp) {
            direction_ = Direction::kDown;
        }
        break;
    case Direction::kLeft:
        if (direction_ != Direction::kRight) {
            direction_ = Direction::kLeft;
        }
        break;
    case Direction::kRight:
        if (direction_ != Direction::kLeft) {
            direction_ = Direction::kRight;
        }
        break;
    }
}
