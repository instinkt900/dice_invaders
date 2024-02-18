#pragma once

#include <cstdint>

struct Rocket {
	void update(float deltaTime);
	void draw();
	void spawn(int32_t _x, int32_t _y);
	void kill();

	int32_t x = 0;
	double y = 0; // floating point for smooth movement
	bool alive = false;
};
