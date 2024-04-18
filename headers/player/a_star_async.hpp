#pragma once


#include <vector>
#include <cmath>
#include <functional>
#include <future>


#include "game_field/cell_type.hpp"


struct ResultCell
{
    ResultCell() = default;
    ResultCell(const int x, const int y, const bool isValid) :
        x(x), y(y), isValid(isValid) {}

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
    AStartAsync(const std::size_t gridWidth, const std::size_t gridHeight);
    ~AStartAsync();

    AStartAsync(const AStartAsync& other) = delete;
    AStartAsync(AStartAsync&& other) = delete;
    AStartAsync& operator=(const AStartAsync& other) = delete;
    AStartAsync& operator=(AStartAsync&& other) = delete;

    void FindPathAsync(int startX, int startY, const CellType goalCellType, 
        const std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field);

	bool ReadyToSearch() const;
    bool HasResult() const;
    ResultCell GetResult();

private:
    ResultCell AStarSearch(Cell* const start, const Cell* const goal) const;

private:
    std::vector<std::vector<Cell*>> grid_{};
    std::future<ResultCell> future_{};
    std::size_t gridHeight_;
    std::size_t gridWidth_;
};
