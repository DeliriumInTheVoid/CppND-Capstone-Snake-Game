#pragma once

class GameModel
{
public:
    GameModel() = default;
    virtual ~GameModel() = default;

public:
    bool game_over() const { return game_over_; }

protected:
    bool game_over_{false}; 
};

class SinglePlayGameModel: public GameModel
{
public:
    SinglePlayGameModel() = default;
    virtual ~SinglePlayGameModel() override {};
};
