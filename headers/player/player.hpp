#pragma once

#include "player/snake.h"
#include "player/player_id.hpp"
#include "player/move_direction.hpp"
#include "player/a_star_async.hpp"
#include "game_field/cell_type.hpp"


struct Winner {
    Winner() : Winner(PlayerId::UNKNOWN, "NoName", 0)
    {}

    Winner(const PlayerId id, const std::string_view name, const int score) :
        id(id), name{ name }, score(score)
    {}

    PlayerId id;
    std::string_view name;
    int score;
};

class Player
{
public:
    Player(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight);
    virtual ~Player() = default;

public:
    void Init();
    void Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field);
    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block);
    virtual void HandleInput(const SDL_Keycode keyCode);

    void AddScore(const int value);

    int Score() const;
    PlayerId Id() const;
    std::string_view Name() const;
    Snake& GetSnake();

protected:
    virtual bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field);

protected:
    std::string_view name_;
    std::unordered_map<SDL_Keycode, Direction> inputMapping_;
    Snake snake_;
    std::size_t gridHeight_;
    std::size_t gridWidth_;

private:
    PlayerId id_;
    int score_{ 0 };
    int hunger_{ 0 };
};
