#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <functional>
#include <future>
#include <set>

struct ResultCell
{
    ResultCell() = default;
    ResultCell(int x, int y, bool isValid) : x(x), y(y), isValid(isValid) {}
    int x{}, y{};
    bool isValid{};
};


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


class AStartAsync {
public:
    AStartAsync(const std::size_t gridWidth, const std::size_t gridHeight) :
        gridHeight_(gridHeight), gridWidth_(gridWidth) {

        if (grid_.empty()) {
            grid_.resize(gridWidth + 1);
            for (std::size_t i = 0; i <= gridWidth; ++i) {
                grid_[i].resize(gridHeight + 1);
                for (std::size_t j = 0; j <= gridHeight; ++j) {
                    grid_[i][j] = new Cell(static_cast<int>(i), static_cast<int>(j));
                }
            }
        }
    }

    ~AStartAsync() {
        for (auto& it : grid_) {
            for (const auto& it2 : it) {
                delete it2;
            }
            it.clear();
        }
        grid_.clear();
    }

    AStartAsync(const AStartAsync& other) = delete;
    AStartAsync(AStartAsync&& other) = delete;
    AStartAsync& operator=(const AStartAsync& other) = delete;
    AStartAsync& operator=(AStartAsync&& other) = delete;

    void findPathAsync(int startX, int startY, const CellType goalCellType, 
        const std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>* const field) {

        auto* startPt = grid_[startX][startY];

        const Cell* goalCell{};
        for (std::size_t i = 0; i <= gridWidth_; ++i) {
            for (std::size_t j = 0; j <= gridHeight_; ++j) {
                const auto& cell = field->at(i).at(j);

                grid_[i][j]->type = cell;
                grid_[i][j]->parent = nullptr;

                if (cell == goalCellType) {
                    goalCell = grid_[i][j];
                }
            }
        }

        if (goalCell) {
            future_ = std::async(std::launch::async, &AStartAsync::aStarSearch, this, startPt, goalCell);
        }
    }

    bool ReadyToSearch() const {

        const bool valid = future_.valid();
        if (!valid) {
            return true;
        }
        const auto status = future_.wait_for(std::chrono::seconds(0));

        return !valid || status == std::future_status::ready;
    }

    bool HasResult() const {
        return future_.valid() && future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    ResultCell GetResult() {
        return future_.get();
    }

private:

    ResultCell aStarSearch(Cell* const start, const Cell* const goal) const {
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

        std::vector<Cell*> path{};

        while (!open.empty()) {
            Cell* current = open.top();
            open.pop();

            if (current == goal) {
                while (current != nullptr) {
                    path.push_back(current);
                    current = current->parent;
                    if (current == goal)
                        break;
                }

                if (path.size() > 1) {
                    path.pop_back();
                }
                auto* cell = path.back();

                return {cell->x, cell->y, true};
            }

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if ((dx == 0 && dy == 0) || (dx != 0 && dy != 0))
                        continue;

                    const int nx = current->x + dx;
                    const int ny = current->y + dy;

                    if (nx >= 0 && nx < static_cast<int>(grid_.size()) &&
                        ny >= 0 && ny < static_cast<int>(grid_[0].size()) &&
                        (grid_[nx][ny]->type == CellType::EMPTY || grid_[nx][ny]->type == CellType::FOOD)) {
                        Cell* neighbor = grid_[nx][ny];
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
    std::vector<std::vector<Cell*>> grid_{};
    std::future<ResultCell> future_{};
    std::size_t gridHeight_;
    std::size_t gridWidth_;
};
