#include "imgui_impl_sdl2.h"
#include "SDL.h"
#include "controller.h"


void Controller::HandleInput(bool &running, const std::unique_ptr<GameStateManager>& state_manager) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    ImGui_ImplSDL2_ProcessEvent(&e);
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      state_manager->HandleInput(e.key.keysym.sym);
    }
  }
}
