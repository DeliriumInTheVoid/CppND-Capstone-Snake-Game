#include "player/player.hpp"


Player::Player(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight) :
    snake_{ gridWidth, gridHeight, id }, gridHeight_{ gridHeight }, gridWidth_{ gridWidth }, id_{ id } {

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

void Player::Init() {

    if (snake_.Alive()) {
        return;
    }
    snake_.Init();
    score_ = 0;
}

void Player::Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
    if (!snake_.Alive()) {
        return;
    }

    UpdateSnake(field);
}

void Player::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
    snake_.Render(sdl_renderer, block);
}

void Player::HandleInput(const SDL_Keycode keyCode) {
    if (const auto& it = inputMapping_.find(keyCode); it != inputMapping_.end()) {
        snake_.MoveTo(it->second);
    }
}

void Player::AddScore(const int value) {
    score_ += value;
}

int Player::Score() const {
    return score_;
}

PlayerId Player::Id() const {
    return id_;
}

std::string_view Player::Name() const {
    return name_;
}

Snake& Player::GetSnake() {
    return snake_;
}

bool Player::UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {
    return snake_.Update(field);
}
