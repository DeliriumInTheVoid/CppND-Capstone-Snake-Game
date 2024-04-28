# Submission overview


## Clone the project
`git clone --recurse-submodules`

## Udacity workspace build
```
make init_env
make build_debug

./build/SnakeGame
```

## Rubric points
All points indicate at least one place in the code, but it is obvious that almost all clauses appear more than once in the code.
### Loops, Functions, I/O
- The project demonstrates an understanding of C++ functions and control structures.
  - __while:__ `game.cpp` line 15 `void Game::Run()`
  - __if:__ `game.cpp` line 31 `void Game::Update()`
  - __else if:__ `controller.cpp` line 12 `void Controller::HandleInput(bool &running, const std::unique_ptr<GameStateManager>& state_manager)`
  - __switch:__ `gamefield_factory.cpp` line 13 `std::unique_ptr<GameField> GameFieldFactory::CreateGameField(const GameStateType type)`
  - __case:__ `gamefield_factory.cpp` line 15, 21, 28 `std::unique_ptr<GameField> GameFieldFactory::CreateGameField(const GameStateType type)`
  - __default:__ `gamefield_factory.cpp` line 35 `std::unique_ptr<GameField> GameFieldFactory::CreateGameField(const GameStateType type)`
  - __return:__ `gameplay-manager.cpp` line 18 `void GamePlayManager::ChangeState(const GameStateType nextStateType)`
  - __for:__ `player_game_field.cpp` line 93 `Winner PlayerGameField::GetWinner()`
  - __continue:__ `a_star_async.cpp` line 113 `ResultCell AStartAsync::AStarSearch(Cell* const start, const Cell* const goal)`
  - __break:__ `main_menu_state.cpp` line 21, 24, 27, 30, 33 `void MainMenuState::HandleScreenEvent(const MainMenuEvent event)`
- The project reads data from a file and process the data, or the program writes data to a file.
  - __read from file:__ `game_save_data.cpp` line 9, 10 `void Game::LoadBestScore()`
  - __write to file:__ `game_save_data.cpp` line 52, 53 `void Game::SaveData()`
- The project accepts user input and processes the input.
  - __user keyboard input:__ `single_player_state.cpp` line 21, 22 `void SinglePlayerGameState::HandleInput(const SDL_Keycode keyCode)`
  - __user mouse input:__ `multiplayer_screen.cpp` line 44, 49, 54 `void MultiplayerGameScreen::Render()`
- The project uses data structures and immutable variables.
  - __data structures:__ `a_star_async.hpp` line 61 `std::vector<std::vector<Cell*>> grid_{};`
  - __immutable variables:__ `a_star_async.hpp` line 34: `void calculateHeuristic(const Cell* const goal)`

### Object Oriented Programming
  - One or more classes are added to the project with appropriate access specifiers for class members.
    - __class attributes:__ `game_field.hpp` line 37-41
    - __class methods:__ `gameplay_manager.hpp` line 23-26
  - Class constructors utilize member initialization lists.
    - __member initialization:__ `player_game_field.cpp` line 8-10 `PlayerGameField::PlayerGameField(std::size_t gridWidth, std::size_t gridHeight)`
  - Classes abstract implementation details from their interfaces.
    - __class interface:__ `game_state_manager.hpp` line 26-29
    - __expected behavior:__ `game_states.hpp` line 58, 64, 70
  - Classes follow an appropriate inheritance hierarchy with virtual and override functions.
    - __inheritance, polymorphism:__ `game_field.hpp` line 20-27
  - Templates generalize functions or classes in the project.
    - __template class:__ `screen.hpp` line 19, 20

### Memory Management
  - The project makes use of references in function declarations.
    - __reference arg:__ `game_states.hpp` line 58 `void Render(SDL_Renderer *sdl_renderer, SDL_Rect& block)`
    - __reference arg:__ `a_star_async.hpp` line 58 `ResultCell AStarSearch(Cell* const start, const Cell* const goal)`
    - __reference attr:__ `renderer.h` line 19, 20
  - The project uses destructors appropriately.
    - __destructor:__ `a_star_async.cpp` line 21-29 `AStartAsync::~AStartAsync()`
  - The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
    - __allocating:__ `a_star_async.cpp` line 15 `AStartAsync::AStartAsync(const std::size_t gridWidth, const std::size_t gridHeight)`
    - __initializing:__ `a_star_async.cpp` line 41, 42 `void AStartAsync::FindPathAsync(int startX, int startY, const CellType goalCellType, const std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field)`
    - __destruction:__ `a_star_async.cpp` line 24 `AStartAsync::~AStartAsync()`
  - The project follows the Rule of 5.
    - __operators, constructors, destructor:__ `a_star_async.hpp` line 42-48
  - The project uses move semantics to move data instead of copying it, where possible.
    - __move semantic:__ `game_states.cpp` line 15, 16 `void GameStateBase::MoveGamePlayManager(std::unique_ptr<GamePlayManager>&& manager)`
  - The project uses smart pointers instead of raw pointers.
    - __unique_ptr:__ `game_states.hpp` line 77

### Concurrency 
  - The project uses multithreading.
    - __multithreading:__ `a_star_async.cpp` line 51 `void AStartAsync::FindPathAsync(int startX, int startY, const CellType goalCellType, const std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field)`
    - __get result:__ `a_star_async.cpp` line 71 `ResultCell AStartAsync::GetResult()`

## Dependencies 
All libraries install with vcpkg. The project uses the following libraries:
- SDL2
- ImGui
- nlohmann json

## Snake Game
Game has the three modes:

* Single player
* Player vs Player
* Player vs AI Player

### Single player
Control the snake with the arrow keys. The snake will grow when it eats the food. The game ends when the snake collides with itself.

### Player vs Player
Control the first snake with the arrow keys and the second snake with the WASD keys. The snakes will grow when they eat the food. The game ends when one of the snakes collides with itself or the other snake.

### Player vs AI Player
Control the snake with the arrow keys. The snake will grow when it eats the food. The game ends when the snake collides with itself or the AI snake.

If snake collides with the other snake, the game will end and despite the score of the collides snake it will lose.

### Controls
- Arrow keys: Move the snake (all game modes)
- WASD keys: Move the second snake (PvP game mode)
- P, Esc: Pause the game (all game modes)

### New features
- Added the new game modes
- Added the AI player
- Added the UI
- Added the best score save/load for all game modes


# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
