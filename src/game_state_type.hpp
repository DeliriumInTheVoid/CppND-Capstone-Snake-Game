#pragma once

enum class GameStateType: unsigned
{
    NONE,
    MAIN_MENU,
    SINGLE_PLAYER_GAME,
    PvP_GAME,
    PvAI_GAME,
    PAUSE_GAME,
    RESUME_GAME,
    RESTART_GAME,
    QUIT_GAME,
};