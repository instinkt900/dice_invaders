#pragma once

#include "Player.h"
#include "AlienGroup.h"
#include "Rocket.h"
#include "EntityBuffer.h"

// the current state of the game
enum class GameState : int32_t {
    Intro,      // title screen/prep screen.
    Game,       // actively playing
    GameOver    // dead
};

// global state for the game.
struct GameContext {
    Player player;
    Rocket rocket;
    AlienGroup alienGroup;
    EntityBuffer entityBuffer;
    GameState gameState;
};

// created in DiceInvaders.cpp
extern GameContext g_context;
