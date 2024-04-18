#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

//#include "snake.h"
#include "game_state/game_state_manager.hpp"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(std::unique_ptr<GameStateManager>& stateManager) const;
  void UpdateWindowTitle(int fps) const;

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
