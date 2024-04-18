#include "player/a_star_async.hpp"

#include <iostream>
#include <queue>
#include <set>

AStartAsync::AStartAsync(const std::size_t gridWidth, const std::size_t gridHeight) :
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

AStartAsync::~AStartAsync() {
    for (auto& it : grid_) {
        for (const auto& it2 : it) {
            delete it2;
        }
        it.clear();
    }
    grid_.clear();
}

void AStartAsync::FindPathAsync(int startX, int startY, const CellType goalCellType,
    const std::unordered_map<std::size_t, std::unordered_map<std::size_t, CellType>>& field) {

    auto* startPt = grid_[startX][startY];

    const Cell* goalCell{};
    for (std::size_t i = 0; i <= gridWidth_; ++i) {
        for (std::size_t j = 0; j <= gridHeight_; ++j) {
            const auto& cell = field.at(i).at(j);

            grid_[i][j]->type = cell;
            grid_[i][j]->parent = nullptr;

            if (cell == goalCellType) {
                goalCell = grid_[i][j];
            }
        }
    }

    if (goalCell) {
        future_ = std::async(std::launch::async, &AStartAsync::AStarSearch, this, startPt, goalCell);
    }
}

bool AStartAsync::ReadyToSearch() const {

    const bool valid = future_.valid();
    if (!valid) {
        return true;
    }
    const auto status = future_.wait_for(std::chrono::seconds(0));

    return !valid || status == std::future_status::ready;
}

bool AStartAsync::HasResult() const {
    return future_.valid() && future_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

ResultCell AStartAsync::GetResult() {
    return future_.get();
}

ResultCell AStartAsync::AStarSearch(Cell* const start, const Cell* const goal) const {
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

            return { cell->x, cell->y, true };
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
