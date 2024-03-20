#pragma once

enum class GameStateType: unsigned
{
    NONE,
    MAIN_MENU,
    SINGLE_PLAYER_GAME,
    PvP_GAME,
    PAUSE_GAME,
    RESTART_GAME,
    QUIT_GAME,
};