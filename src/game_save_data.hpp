#pragma once

#include <fstream>

#include <nlohmann/json.hpp>

#include "game_state_type.hpp"

class GameSaveData {
public:
    GameSaveData() = default;
    ~GameSaveData() = default;

public:
    void PreloadData() {
        nlohmann::json saveDataJson;
        std::ifstream saveDataFile("data/save.json");
        saveDataFile >> saveDataJson;

        for (auto& [key, value] : saveDataJson.items()) {
            GameStateType state{ GameStateType::NONE };
            if (key == "single") {
                state = GameStateType::SINGLE_PLAYER_GAME;
            }
            else if (key == "pvp") {
                state = GameStateType::PvP_GAME;
            }
            else if (key == "pvai") {
                state = GameStateType::PvAI_GAME;
            }

            auto const bestScore = value["best_score"].get<int>();

            if (state != GameStateType::NONE) {
                bestScoreMap_[state] = bestScore;
            }
        }
    }

    void SaveData() {
        nlohmann::json saveDataJson;

        for (auto& [state, bestScore] : bestScoreMap_) {
            std::string key;
            if (state == GameStateType::SINGLE_PLAYER_GAME) {
                key = "single";
            }
            else if (state == GameStateType::PvP_GAME) {
                key = "pvp";
            }
            else if (state == GameStateType::PvAI_GAME) {
                key = "pvai";
            }

            nlohmann::json stateJson;
            stateJson["best_score"] = bestScore;
            saveDataJson[key] = stateJson;
        }

        std::ofstream saveDataFile("data/save.json");
        saveDataFile << saveDataJson.dump(2);
    }

    void SetBestScore(const GameStateType state, const int value, const bool flush=true) {
        bestScoreMap_[state] = value;
        if (flush) {
            SaveData();
        }
    }

    int GetBestScore(const GameStateType state) {
        if (bestScoreMap_.find(state) == bestScoreMap_.end()) {
            PreloadData();
        }
        return bestScoreMap_[state];
    }

private:
    std::unordered_map<GameStateType, int> bestScoreMap_{};
};
