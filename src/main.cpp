#include <iostream>
#include <memory>

#include "controller.h"
#include "game.h"
#include "renderer.h"

#include "game_states.hpp"
#include "game_state_factory.hpp"
#include "game_state_manager.hpp"
#include "gameplay_manager.hpp"

int main(int argc, char* argv[]) {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;

  std::unique_ptr<GameFieldFactory> gameFieldFactory = std::make_unique<GameFieldFactory>(kGridWidth, kGridHeight);
  std::unique_ptr<GamePlayManager> gamePlayManager = std::make_unique<GamePlayManager>(std::move(gameFieldFactory));
  std::unique_ptr<GameStateFactory> gameStateFactory = std::make_unique<GameStateFactory>(kGridWidth, kGridHeight);
  std::unique_ptr<GameStateManager> gameStateManager = std::make_unique<GameStateManager>(std::move(gameStateFactory), std::move(gamePlayManager));

  Game game(kGridWidth, kGridHeight, std::move(gameStateManager));
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  return 0;
}