#pragma once

#include <memory>

class GameStateBase;
enum class GameStateType : unsigned;


class GameStateFactory
{
public:
    GameStateFactory(const std::size_t screenWidth, const std::size_t screenHeight);
    ~GameStateFactory() = default;

public:
    std::unique_ptr<GameStateBase> CreateGameState(const GameStateType type);

private:
    std::size_t screenWidth_;
    std::size_t screenHeight_;
};
