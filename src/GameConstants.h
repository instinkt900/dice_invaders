#pragma once

#include "Engine.h"
#include <cstdint>

static int32_t const AlienCols = 12;                                        // number of alien columns in the group
static int32_t const AlienRows = 5;                                         // number of rows in the alien group
static int32_t const MaxAliens = AlienCols * AlienRows;                     // convenience for the max number of aliens possible
static int32_t const MaxBombs = 10;                                         // limit for the number of bombs on screen at a time
static int32_t const AlienBuffer = 5;                                       // number of pixels to inset the alien sprite in each "cell"
static int32_t const AlienCellSize = Engine::SpriteSize + AlienBuffer * 2;  // size of each "cell" in the alien group
static int32_t const AlienMoveDistance = AlienCellSize / 2;                 // distance for the aliens to move each step
static int32_t const MaxEntities = MaxAliens + MaxBombs + 2 + 10;           // total number of renderable entities. aliens, bombs, player and rocket, plus some extra just for quick additions

static float const AlienMoveDelayMax = 1.0f;                                // the maximum delay between alien steps
static float const AlienMoveDelayMin = 0.01f;                               // the minimum delay between alien steps. we lerp between these two as aliens die
static float const AlienFireDelayMax = 3.0f;                                // max delay between alien bomb spawns
static float const AlienFireDelayMin = 0.01f;                               // min delay between alien bomb spawns. lerped with step delay as aliens die
static float const BombSpeed = 130.0f;                                      // speed of bombs dropping. pixels per second
static float const ResetDelay = 3.0f;                                       // delay between clearing the aliens and new aliens appearing

static float const PlayerSpeed = 600.0f;                                    // speed of the player ship. pixels per second
static float const PlayerFireDelay = 0.2f;                                  // min delay between player shots. only one rocket alive at a time
static int32_t const ScoreBufferSize = 50;                                  // text buffer for score string formatting
static int32_t const PlayerHealth = 3;                                      // number of ships available to the player (lives)

static float const RocketSpeed = 800.0f;                                    // speed of the player rocket. pixels per second
static int32_t const ScorePerAlien = 100;                                   // number of points per alien kill
