#pragma once

#include <random>

#include "cell_type.hpp"
#include "game_field/game_field.hpp"
#include "player/player_id.hpp"


class PlayerGameField : public GameField
{
public:
    PlayerGameField(std::size_t gridWidth, std::size_t gridHeight);
    ~PlayerGameField() override = default;

public:
    void Init() override;
    GameFieldState Update() override;
    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override;
    void HandleInput(const SDL_Keycode key_code) override;
    int PlayerScore(const PlayerId player_id) const override;
    virtual Winner GetWinner() const override;

private:
    void PlaceFood();

private:
    SDL_Point food_{};
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> fieldCells_{};

    std::random_device randomDev_;
    std::mt19937 randomEngine_;
    std::uniform_int_distribution<int> randomWidth_;
    std::uniform_int_distribution<int> randomHeight_;
};
