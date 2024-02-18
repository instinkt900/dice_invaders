#pragma once

#include "GameConstants.h"
#include <array>

// represents the whole alien group and its bombs
struct AlienGroup {
	struct Alien {
		int32_t x;
		int32_t y;
		bool alive;
	};

	struct Bomb {
		int32_t x;
		float y;	// so we get smooth falling movement
	};

	// Set up all the alien data so theyre arranged in a nice big group in the middle of the screen
	void setup();
	// moves the aliens in one large step in the direction currently set
	void update(float deltaTime);
	void updateAliens(float deltaTime);
	void updateFiring(float deltaTime);
	void spawnBomb(int32_t x, int32_t y);
	void updateBombs(float deltaTime);
	void destroyBomb(int32_t index);
	// draw all alive aliens
	void draw();
	void kill(int32_t index);

	std::array<Alien, AlienCols * AlienRows> aliens;
	std::array<Bomb, MaxBombs> bombs;
	int32_t bombCount = 0;
	int32_t aliveCount = 0;
	int32_t directionX = 0;
	int32_t directionY = 0;
	int32_t nextDirectionX = 0;
	float moveTimer = 0;
	float fireDelay = 0;
	float resetTimer = 0;
};
