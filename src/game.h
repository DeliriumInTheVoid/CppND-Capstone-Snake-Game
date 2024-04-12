#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "controller.h"
#include "renderer.h"
#include "game_state_manager.hpp"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height, std::unique_ptr<GameStateManager>&& game_state_manager);
  void Run(Controller const &controller, const Renderer &renderer,
           std::size_t targetFrameDuration);

private:
  std::unique_ptr<GameStateManager> gameStateManager_;
};

#endif
