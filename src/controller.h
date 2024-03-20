#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <memory>

#include "snake.h"
#include "game_state_manager.hpp"

class Controller {
 public:
  void HandleInput(bool &running, const std::unique_ptr<GameStateManager>& state_manager) const;
};

#endif