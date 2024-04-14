#pragma once

#include "snake.h"
#include "player_id.hpp"
#include "cell_type.hpp"
#include "move_direction.hpp"

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <functional>
#include <set>

struct Cell {
    int x, y; // Coordinates
    int f, g, h; // Cost values
    Cell* parent; // Pointer to the parent cell

    CellType type = CellType::EMPTY;

    Cell(const int x, const int y) : x(x), y(y), f(0), g(0), h(0), parent(nullptr) {}

    // Calculate the heuristic value (Euclidean distance)
    void calculateHeuristic(const Cell* const goal) {
        h = static_cast<int>(sqrt(pow((x - goal->x), 2) + pow((y - goal->y), 2)));
    }
};

class Player
{
public:
    Player(const PlayerId id, const std::size_t gridWidth, const std::size_t gridHeight) :
        snake{ gridWidth, gridHeight, id }, gridHeight_{gridHeight}, gridWidth_{ gridWidth }, id{id}
    {
        if (id == PlayerId::PLAYER_1) {
            inputMapping_ = {
                {SDLK_UP, Direction::kUp},
                {SDLK_DOWN, Direction::kDown},
                {SDLK_LEFT, Direction::kLeft},
                {SDLK_RIGHT, Direction::kRight}
            };
        }
        else if (id == PlayerId::PLAYER_2)
        {
            inputMapping_ = {
                { SDLK_w, Direction::kUp },
                { SDLK_s, Direction::kDown },
                { SDLK_a, Direction::kLeft },
                { SDLK_d, Direction::kRight }
            };
        }
    }

    virtual ~Player() = default;

public:
    void Init() {

        if (snake.Alive()) {
            return;
        }
        snake.Init();
        score = 0;
    }

    void Update(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>> *const field) {
        if (!snake.Alive()) {
            return;
        }
        
        UpdateSnake(field);
    }

    virtual void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) {
        snake.Render(sdl_renderer, block);
    }

    virtual void HandleInput(const SDL_Keycode keyCode) {
        if (const auto& it = inputMapping_.find(keyCode); it != inputMapping_.end()) {
            snake.MoveTo(it->second);
        }
    }

    void AddScore(const int value) {
        score += value;
    }

    int Score() const { return score; }
    PlayerId Id() const { return id; }
    Snake& GetSnake() { return snake; }

protected:
    virtual bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>* const field) {
        return snake.Update(field);
    }

protected:
    std::unordered_map<SDL_Keycode, Direction> inputMapping_;
    Snake snake;
    size_t gridHeight_;
    size_t gridWidth_;

private:
    PlayerId id;
    int score{ 0 };
    int hunger{ 0 };
};


class PlayerAI : public Player
{
public:
    PlayerAI(const PlayerId id, const std::size_t grid_width, const std::size_t grid_height) :
        Player{ id, grid_width, grid_height }
    {}

    virtual ~PlayerAI() override {
        for (auto& it : grid_)
        {
            for (const auto& it2 : it) {
                delete it2;
            }
            it.clear();
        }
        grid_.clear();
    }

public:
    //void Render(SDL_Renderer* sdl_renderer, SDL_Rect& block) override {
    //    Player::Render(sdl_renderer, block);

    //    if (!path_.empty()) {
    //        path_.erase(path_.begin());
    //        for (const auto* cell : path_) {
    //            SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 255, 255);
    //            block.x = cell->x * block.w;
    //            block.y = cell->y * block.h;
    //            SDL_RenderFillRect(sdl_renderer, &block);
    //        }
    //    }
    //}

    virtual void HandleInput(const SDL_Keycode key_code) override
    {
    }

protected:
    bool UpdateSnake(std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>* const field) override{
        if (!Player::UpdateSnake(field)) {
            return false;
        }

        if (grid_.empty()) {
            grid_.resize(gridWidth_ + 1);
            for (std::size_t i = 0; i <= gridWidth_; ++i) {
                grid_[i].resize(gridHeight_ + 1);
                for (std::size_t j = 0; j <= gridHeight_; ++j) {
                    grid_[i][j] = new Cell(static_cast<int>(i), static_cast<int>(j));
                }
            }
        }

        const Cell* foodPt{};
        for (std::size_t i = 0; i <= gridWidth_; ++i) {
            for (std::size_t j = 0; j <= gridHeight_; ++j) {
                const auto& cell = field->at(i).at(j);

                grid_[i][j]->type = cell;
                grid_[i][j]->parent = nullptr;

                if (cell == CellType::FOOD) {
                    foodPt = grid_[i][j];
                }
            }
        }

        if (foodPt) {
            const auto path = AStar(grid_[snake.HeadX()][snake.HeadY()], foodPt, grid_);
            //path_ = path;

            if (path.empty()) {
                return true;
            }
            const auto* cell = path.size() > 1 ? path[1] : path[0];
            field->at(cell->x).at(cell->y) = CellType::MOVE_GOAL;
            if (cell->x > snake.HeadX()) {
                snake.MoveTo(Direction::kRight);
            }
            else if (cell->x < snake.HeadX()) {
                snake.MoveTo(Direction::kLeft);
            }
            else if (cell->y > snake.HeadY()) {
                snake.MoveTo(Direction::kDown);
            }
            else if (cell->y < snake.HeadY()) {
                snake.MoveTo(Direction::kUp);
            }

            //const auto distX = std::abs(snake.HeadX() - foodPt.x);
            //const auto distY = std::abs(snake.HeadY() - foodPt.y);

            //if (distX > distY) {
            //    if (snake.HeadX() < foodPt.x) {
            //        snake.MoveTo(SDLK_d);
            //    }
            //    else {
            //        snake.MoveTo(SDLK_a);
            //    }
            //}
            //else {
            //    if (snake.HeadY() < foodPt.y) {
            //        snake.MoveTo(SDLK_s);
            //    }
            //    else {
            //        snake.MoveTo(SDLK_w);
            //    }
            //}
        }

        return true;
    }


    std::vector<Cell*> AStar(Cell* const start, const Cell* const goal, const std::vector<std::vector<Cell*>>& grid) {

        std::priority_queue<Cell*, std::vector<Cell*>, std::function<bool(Cell*, Cell*)>> open(
            [](const Cell* a, const Cell* b) {
                return a->f > b->f;
            });

        std::set<Cell*> visitedCells;

        start->g = 0;
        start->calculateHeuristic(goal);
        start->f = start->g + start->h;
        open.push(start);
        visitedCells.insert(start);

        while (!open.empty()) {
            Cell* current = open.top();
            open.pop();

            if (current == goal) {
                std::vector<Cell*> path;
                while (current != nullptr) {
                    path.push_back(current);
                    current = current->parent;
                    if (current == goal)
                        break;
                }
                std::reverse(path.begin(), path.end());
                return path;
            }

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
                        continue;

                    const int nx = current->x + dx;
                    const int ny = current->y + dy;

                    if (nx >= 0 && nx < static_cast<int>(grid.size()) &&
                        ny >= 0 && ny < static_cast<int>(grid[0].size()) &&
                        (grid[nx][ny]->type == CellType::EMPTY || grid[nx][ny]->type == CellType::FOOD)) {
                        Cell* neighbor = grid[nx][ny];
                        const int newG = current->g + 1; //uniform cost for simplicity

                        if (neighbor == current || current->parent == neighbor || neighbor->parent == current) {
                            continue;
                        }

                        if (newG < neighbor->g || neighbor->parent == nullptr) {
                            neighbor->g = newG;
                            neighbor->calculateHeuristic(goal);
                            neighbor->f = neighbor->g + neighbor->h;

                            neighbor->parent = current;
                            if (visitedCells.find(neighbor) == visitedCells.end()) {
                                open.push(neighbor);
                                visitedCells.insert(neighbor);
                            }
                        }
                    }
                }
            }
        }

        return {};
    }

private:
    //std::vector<Cell*> path_{};
    std::vector<std::vector<Cell*>> grid_{};
};
