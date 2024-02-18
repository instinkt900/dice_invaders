#pragma once

#include "GameConstants.h"

struct Player {
	void setup();
	void update(float deltaTime, Engine& engine);
	void draw();
	void addScore(int32_t amount);
	void updateScore();
	void hit();

	float x = 0;	// for smooth movement
	int32_t y = 0;	// this stays constant through the game
	int32_t health = PlayerHealth;
	float fireDelay = 0;
	int32_t score = 0;
	char scoreBuffer[ScoreBufferSize];	// used for formatting the score for display
};
