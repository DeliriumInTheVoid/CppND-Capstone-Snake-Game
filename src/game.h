#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>

#include "SDL.h"

#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "game_state_manager.hpp"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height, std::unique_ptr<GameStateManager>&& game_state_manager);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t targetFrameDuration);

private:
  //void PlaceFood();
  //void Update();

private:
  std::unique_ptr<GameStateManager> gameStateManager_;
  //Snake snake;
  //Snake snake2;
  //SDL_Point food;

  // std::random_device dev;
  // std::mt19937 engine;
  // std::uniform_int_distribution<int> random_w;
  // std::uniform_int_distribution<int> random_h;

  //int score{0};
};

#endif