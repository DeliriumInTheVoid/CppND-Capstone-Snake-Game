#pragma once

#include "game_field/game_field.hpp"


class NullGameField : public GameField
{
public:
    NullGameField()
        : GameField(0, 0)
    {}

    virtual ~NullGameField() override {}

public:
    void Init() override {}
    GameFieldState Update() override { return GameFieldState::NONE; }
    void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {}
    void HandleInput(SDL_Keycode keyCode) override {}
    int PlayerScore(PlayerId PlayerId) const override { return 0; }
};
