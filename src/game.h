#ifndef GAME_H
#define GAME_H

#include <memory>

#include "controller.h"
#include "renderer.h"
#include "game_state_manager.hpp"

class Game
{
public:
  Game(std::unique_ptr<GameStateManager>&& gameStateManager);

  void Run(Controller const &controller, const Renderer &renderer,
           std::size_t targetFrameDuration);

private:
  std::unique_ptr<GameStateManager> gameStateManager_;
};

#endif
