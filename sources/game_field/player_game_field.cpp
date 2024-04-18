#include "game_field/player_game_field.hpp"

PlayerGameField::PlayerGameField(std::size_t gridWidth, std::size_t gridHeight) : GameField(gridWidth, gridHeight),
randomEngine_(randomDev_()),
randomWidth_(0, static_cast<int>(gridWidth - 1)),
randomHeight_(0, static_cast<int>(gridHeight - 1))
{}

inline void PlayerGameField::Init()
{
    for (const auto& [id, player] : players_) {
        player->Init();
    }

    for (std::size_t i = 0; i <= gridWidth_; ++i) {
        fieldCells_.emplace(i, std::unordered_map<std::size_t, CellType>{});
        for (std::size_t j = 0; j <= gridHeight_; ++j) {
            auto& row = fieldCells_[i];
            row.emplace(j, CellType::EMPTY);
        }
    }

    PlaceFood();

    gameFieldState_ = GameFieldState::GAME_PLAY;
}

GameFieldState PlayerGameField::Update()
{

    if (gameFieldState_ != GameFieldState::GAME_PLAY) {
        return gameFieldState_;
    }

    for (const auto& [id, player] : players_) {
        player->Update(fieldCells_);
        Snake& snake = player->GetSnake();

        if (!snake.Alive()) {
            gameFieldState_ = GameFieldState::GAME_OVER;
            return gameFieldState_;
        }

        if (snake.GotFood(food_)) {
            fieldCells_[food_.x][food_.y] = CellType::EMPTY;
            player->AddScore(1);
            snake.GrowBody();
            snake.SpeedUp(0.01f);
            PlaceFood();
        }
    }

    return gameFieldState_;
}

void PlayerGameField::Render(SDL_Renderer* sdl_renderer, SDL_Rect& block)
{
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

void PlayerGameField::HandleInput(const SDL_Keycode key_code)
{
    for (const auto& [id, player] : players_) {
        player->HandleInput(key_code);
    }
}

int PlayerGameField::PlayerScore(const PlayerId player_id) const
{
    return players_.find(player_id)->second->Score();
}

Winner PlayerGameField::GetWinner() const
{

    if (gameFieldState_ != GameFieldState::GAME_OVER) {
        return {};
    }

    for (const auto& [id, player] : players_) {
        Snake& snake = player->GetSnake();

        if (snake.Alive()) {
            return { player->Id(), player->Name(), player->Score() };
        }
    }

    return {};
}

void PlayerGameField::PlaceFood()
{
    while (true)
    {
        const int x = randomWidth_(randomEngine_);
        const int y = randomHeight_(randomEngine_);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (fieldCells_[x][y] == CellType::EMPTY) {
            food_.x = x;
            food_.y = y;
            fieldCells_[x][y] = CellType::FOOD;
            return;
        }
    }
}
