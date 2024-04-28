#include <unordered_map>

#include "player/ai_player.hpp"
#include "game_field/cell_type.hpp"
#include "player/move_direction.hpp"
#include "player/player.hpp"
#include "player/player_id.hpp"

PlayerAI::PlayerAI(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight) :
    Player{ id, gridWidth, gridHeight }, aStar_{ gridWidth, gridHeight } {
    name_ = "AI Player";
}

bool PlayerAI::UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
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
        aStar_.FindPathAsync(snake_.HeadX(), snake_.HeadY(), CellType::FOOD, field);
    }

    return true;
}
