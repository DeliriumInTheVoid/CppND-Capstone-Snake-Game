#include "SDL.h"
#include "game_core/game.h"


Game::Game(std::unique_ptr<GameStateManager>&& gameStateManager)
    : gameStateManager_(std::move(gameStateManager))
{}

void Game::Run(Controller const& controller, const Renderer& renderer,
    std::size_t targetFrameDuration) {
    Uint32 titleTimestamp = SDL_GetTicks();
    int frame_count = 0;
    bool running = true;

    while (running && gameStateManager_->CurrentStateType() != GameStateType::QUIT_GAME) {
        const Uint32 frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(running, gameStateManager_);
        gameStateManager_->Update();
        renderer.Render(gameStateManager_);

        const Uint32 frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        const Uint32 frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - titleTimestamp >= 1000) {
            renderer.UpdateWindowTitle(frame_count);
            frame_count = 0;
            titleTimestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < targetFrameDuration) {
            SDL_Delay(static_cast<Uint32>(targetFrameDuration) - frame_duration);
        }
    }
}
