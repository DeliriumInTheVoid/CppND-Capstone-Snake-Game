#include <iostream>
#include <memory>

#include "controller.h"
#include "game.h"
#include "renderer.h"

#include "game_states.hpp"
#include "game_state_factory.hpp"
#include "game_state_manager.hpp"
#include "gameplay_manager.hpp"
#include "game_save_data.hpp"

int main(int argc, char* argv[]) {
    constexpr std::size_t kFramesPerSecond{ 60 };
    constexpr std::size_t kMsPerFrame{ 1000 / kFramesPerSecond };
    constexpr std::size_t kScreenWidth{ 640 };
    constexpr std::size_t kScreenHeight{ 640 };
    constexpr std::size_t kGridWidth{ 32 };
    constexpr std::size_t kGridHeight{ 32 };

    const Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    constexpr Controller controller{};

    auto gameFieldFactory = std::make_unique<GameFieldFactory>(kGridWidth, kGridHeight);

    auto gameStateManager = std::make_unique<GameStateManager>(
        std::make_unique<GameStateFactory>(kScreenWidth, kScreenHeight),
        std::make_unique<GamePlayManager>(std::move(gameFieldFactory)),
        std::make_unique<GameSaveData>()
    );

    Game game{ std::move(gameStateManager) };
    game.Run(controller, renderer, kMsPerFrame);
    std::cout << "Game has terminated successfully!\n";
    return 0;
}
