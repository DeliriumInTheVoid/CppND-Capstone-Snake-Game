#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

//#include "snake.h"
#include "game_state_manager.hpp"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  //void RenderSnake(Snake const &snake, SDL_Rect &block);
  void Render(std::unique_ptr<GameStateManager>& stateManager/*Snake const &snake, Snake const &snake2, SDL_Point const &food*/) const;
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