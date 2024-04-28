#pragma once

#include <unordered_map>

#include "game_state/game_state_type.hpp"

class GameSaveData {
public:
    GameSaveData() = default;
    ~GameSaveData() = default;

public:
    void PreloadData();
    void SaveData();
    void SetBestScore(const GameStateType state, const int value, const bool flush=true);
    int GetBestScore(const GameStateType state);

private:
    static const std::string SAVE_FILE_PATH;

private:
    std::unordered_map<GameStateType, int> bestScoreMap_{};
};
