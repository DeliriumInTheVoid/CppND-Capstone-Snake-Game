#pragma once

#include <string>
#include "imgui.h"
#include "player.hpp"


class ScreenConsts
{
public:
    static constexpr int BUTTON_WIDTH = 300;
    static constexpr int BUTTON_HEIGHT = 20;
    static constexpr int BUTTONS_VGAP = 40;

    static constexpr ImVec2 BUTTON_SIZE{ BUTTON_WIDTH, BUTTON_HEIGHT };
};

using ScreenEvent = unsigned int;

template <typename T>//, std::enable_if_t<std::is_enum_v<ScreenEvent>> 
class Screen
{
public:
    Screen() = default;
    virtual ~Screen() {}

public:
    T ScreenEvent() const { return screenEvent_; }

public:
    virtual void Render() = 0;
    virtual void Update() {}
    virtual void Init() {}

protected:
    T screenEvent_{};
};

enum class MainMenuEvent : ScreenEvent
{
    NONE = 0,
    MAIN_MENU,
    SINGLE_PLAYER_GAME,
    PvP_GAME,
    QUIT_GAME,
};

enum class SinglePlayerGameEvent : ScreenEvent
{
    NONE = 0,
    RESTART_GAME,
    MAIN_MENU,
    QUIT_GAME,
};

enum class MultiplayerGameEvent : ScreenEvent
{
    NONE = 0,
    RESTART_GAME,
    MAIN_MENU,
    QUIT_GAME,
};

enum class PauseGameEvent : ScreenEvent
{
    NONE = 0,
    RESUME_GAME,
    QUIT_GAME,
};

class MainMenuScreen : public Screen<MainMenuEvent>
{
public:
    MainMenuScreen() = default;
    virtual ~MainMenuScreen() override {}
    virtual void Render() override
    {
        screenEvent_ = MainMenuEvent::NONE;

        const int vGap{ 4 };
        const int buttonsQty{ 4 };

        const ImVec2 windowSize{
            ScreenConsts::BUTTON_SIZE.x,
            ScreenConsts::BUTTON_SIZE.y * buttonsQty + vGap * (buttonsQty - 1)
        };
        const ImVec2 windowPos{ (640 - windowSize.x) * 0.5f, 200 };
        const auto screenSize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(screenSize);

        ImGui::Begin("Game Menu", nullptr,
                    ImGuiWindowFlags_NoCollapse | 
                    ImGuiWindowFlags_NoResize | 
                    ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoBackground | 
                    ImGuiWindowFlags_NoTitleBar
        );

        // Center text by using SameLine
        //ImGui::Dummy(ImVec2(centerPosX, 0.0f)); // Move cursor to center

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("Single Player", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MainMenuEvent::SINGLE_PLAYER_GAME;
        }

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("PvP", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MainMenuEvent::PvP_GAME;
        }

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("Player vs AI", ScreenConsts::BUTTON_SIZE)) {

        }

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
            screenEvent_ = MainMenuEvent::QUIT_GAME;
        }

        ImGui::End();
    }

    virtual void Update() override
    {
    }
};

class SinglePlayerGameScreen : public Screen<SinglePlayerGameEvent>
{
public:
    SinglePlayerGameScreen() = default;
    virtual ~SinglePlayerGameScreen() override = default;

public:
    void Score(const int score) {
        if (score_ == score) {
           return;
        }
        score_ = score;
        scoreText_ = "Score: " + std::to_string(score);
    }

    void Init() override {
        Score(0);
        gameOver_ = false;
    }

    void GameOver(){
        gameOver_ = true;
    }

    virtual void Render() override
    {
        screenEvent_ = SinglePlayerGameEvent::NONE;

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Score Label", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBackground | 
            ImGuiWindowFlags_NoSavedSettings
        );
        ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - ImGui::CalcTextSize(scoreText_.c_str()).x) / 2, 10));
        ImGui::Text("%s", scoreText_.c_str());

        if (gameOver_) {
            ImGui::SetCursorPos({(640 - ScreenConsts::BUTTON_WIDTH) / 2, 100 });
            if (ImGui::Button("Restart Game", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = SinglePlayerGameEvent::RESTART_GAME;
            }

            ImGui::SetCursorPosX((640 - ScreenConsts::BUTTON_WIDTH) / 2);
            if (ImGui::Button("Main Menu", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = SinglePlayerGameEvent::MAIN_MENU;
            }

            ImGui::SetCursorPosX((640 - ScreenConsts::BUTTON_WIDTH) / 2);
            if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = SinglePlayerGameEvent::QUIT_GAME;
            }
        }

        ImGui::End();
    }

private:
    bool gameOver_{ false };
    int score_{ -1 };
    std::string scoreText_;
};

class PvPGameScreen : public Screen<MultiplayerGameEvent>
{
public:
    PvPGameScreen() = default;
    virtual ~PvPGameScreen() override = default;

public:
    void Score(const int player1_score, const int player2_score) {
        UpdateScore(scorePlayer1, player1_score, scoreTextPlayer1);
        UpdateScore(scorePlayer2, player2_score, scoreTextPlayer2);
    }

    void Init() override {
        winner_ = PlayerId::UNKNOWN;
        screenEvent_ = MultiplayerGameEvent::NONE;
        Score(0, 0);
    }

    virtual void Render() override
    {
        screenEvent_ = MultiplayerGameEvent::NONE;

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Score Label", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoSavedSettings
        );

        ImGui::SetCursorPos(ImVec2(10, 10));
        ImGui::Text("%s", scoreTextPlayer1.c_str());

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - ImGui::CalcTextSize(scoreTextPlayer2.c_str()).x - 10, 10));
        ImGui::Text("%s", scoreTextPlayer2.c_str());

        if (winner_ != PlayerId::UNKNOWN) {
            ImGui::SetCursorPos({ (640 - ScreenConsts::BUTTON_WIDTH) / 2, 100 });
            if (ImGui::Button("Restart Game", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = MultiplayerGameEvent::RESTART_GAME;
            }

            ImGui::SetCursorPosX((640 - ScreenConsts::BUTTON_WIDTH) / 2);
            if (ImGui::Button("Main Menu", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = MultiplayerGameEvent::MAIN_MENU;
            }

            ImGui::SetCursorPosX((640 - ScreenConsts::BUTTON_WIDTH) / 2);
            if (ImGui::Button("Quit", ScreenConsts::BUTTON_SIZE)) {
                screenEvent_ = MultiplayerGameEvent::QUIT_GAME;
            }
        }

        ImGui::End();
    }

    void Winner(const PlayerId player) {
        winner_ = player;
    }

private:
    void UpdateScore(int& score, const int& new_score, std::string& score_text) {

        if (new_score == score) {
            return;
        }

        score = new_score;
        score_text = "Score: " + std::to_string(score);
    }

private:
    int scorePlayer1{ -1 };
    std::string scoreTextPlayer1;

    int scorePlayer2{ -1 };
    std::string scoreTextPlayer2;

    PlayerId winner_{ PlayerId::UNKNOWN };
};

class PauseGameScreen : public Screen<PauseGameEvent>
{
public:
    PauseGameScreen() = default;
    virtual ~PauseGameScreen() override {}

    virtual void Render() override
    {
        screenEvent_ = PauseGameEvent::NONE;
        // Create the game menu
        ImGui::Begin("Game Menu", nullptr,
                    ImGuiWindowFlags_NoCollapse | 
                    ImGuiWindowFlags_NoResize | 
                    ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoBackground | 
                    ImGuiWindowFlags_NoTitleBar
        );

        float buttonWidth = 300;
        float centerPosX = (ImGui::GetWindowSize().x - buttonWidth) * 0.5f;
        
        // Center text by using SameLine
        //ImGui::Dummy(ImVec2(centerPosX, 0.0f)); // Move cursor to center

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("Resume", ImVec2(300, 0))) {
            // Handle Resume action
            //nextStateType_ = GameStateType::SINGLE_PLAYER_GAME;
            screenEvent_ = PauseGameEvent::RESUME_GAME;
        }

        //ImGui::SetCursorPosX(centerPosX);
        if (ImGui::Button("Main Menu", ImVec2(300, 0))) {
            // Handle Main Menu action
            //nextStateType_ = GameStateType::MAIN_MENU;
            screenEvent_ = PauseGameEvent::QUIT_GAME;
        }

        ImGui::End();
    }
};
