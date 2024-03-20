#include <cmath>
#include "snake.h"

void Snake::Init() {
    alive = true;
    size = 1;
    speed = 0.1f;
    //head_x = grid_width / 2;
    //head_y = grid_height / 2;
    head_x = initial_position.x;
    head_y = initial_position.y;
    body.clear();
    direction = Direction::kUp;
}

void Snake::Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field) {

    if (!alive) {
        return;
    }

    SDL_Point prev_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y) };  // We first capture the head's cell before updating.
    UpdateHead();
    SDL_Point current_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y) };  // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
        UpdateBody(current_cell, prev_cell, field);
    }
}

bool Snake::GotFood(const SDL_Point& food) const
{
    const int new_x = static_cast<int>(head_x);
    const int new_y = static_cast<int>(head_y);

    // Check if the snake head has reached the food
    if (new_x == food.x && new_y == food.y) {
        return true;
    }

    return false;
}

void Snake::UpdateHead() {
    switch (direction) {
    case Direction::kUp:
        head_y -= speed;
        break;

    case Direction::kDown:
        head_y += speed;
        break;

    case Direction::kLeft:
        head_x -= speed;
        break;

    case Direction::kRight:
        head_x += speed;
        break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point& current_head_cell, SDL_Point& prev_head_cell, std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field) {
    // Add previous head location to vector
    body.push_back(prev_head_cell);

    if (!growing) {
        const auto& tail = body[0];
        field->at(tail.y).at(tail.x) = CellType::EMPTY;
        // Remove the tail from the vector.
        body.erase(body.begin());
    }
    else {
        growing = false;
        size++;
    }

    // Check if the snake has died.
    for (auto const& item : body) {
        field->at(item.y).at(item.x) = cell_type;
    }

    if (const auto cell_for_head = field->at(current_head_cell.y).at(current_head_cell.x);
        cell_for_head == CellType::SNAKE_P1 || cell_for_head == CellType::SNAKE_P2) {
        alive = false;
    }
    else {
        field->at(current_head_cell.y).at(current_head_cell.x) = cell_type;
    }
}

void Snake::GrowBody() { growing = true; }

void Snake::SpeedUp(const float value)
{
    speed += value;
}

void Snake::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const& point : body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(head_x) * block.w;
    block.y = static_cast<int>(head_y) * block.h;
    if (alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Snake::HandleInput(const SDL_Keycode key_code)
{
    if (const auto& it = input_mapping.find(key_code); it != input_mapping.end())
    {
        switch (it->second)
        {
        case Direction::kUp:
            if (direction != Direction::kDown) {
                direction = Direction::kUp;
            }
            break;
        case Direction::kDown:
            if (direction != Direction::kUp) {
                direction = Direction::kDown;
            }
            break;
        case Direction::kLeft:
            if (direction != Direction::kRight) {
                direction = Direction::kLeft;
            }
            break;
        case Direction::kRight:
            if (direction != Direction::kLeft) {
                direction = Direction::kRight;
            }
            break;
        }
    }
}
