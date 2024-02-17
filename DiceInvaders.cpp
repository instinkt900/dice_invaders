#include "Engine.h"
#include <cstdint>
#include <array>
#include <limits.h>
#include <stdio.h>

static const int32_t AlienCols = 12;
static const int32_t AlienRows = 5;
static const int32_t AlienBuffer = 5;
static const int32_t AlienCellSize = Engine::SpriteSize + AlienBuffer * 2;
static const int32_t AlienMoveDistamce = AlienCellSize;
static const double AlienMoveDelay = 1.0;

static const double PlayerSpeed = 600.0;
static const double RocketSpeed = 800.0;

bool gameOver = false;

struct Alien {
	int32_t x;
	int32_t y;
};

// using a namespace here rather than a class since we wont be needing multiples/instances
// ie. this isnt really a type
namespace AlienData {
	// the alien buffer is a tightly packed buffer of alive aliens. all alive aliens
	// are located at the start of the buffer up to 'aliveCount'. all extra entries
	// are invalid. The idea here being to keep the alien data packed to allow cache
	// to zip over it quickly.
	std::array<Alien, AlienCols * AlienRows> aliens;
	int32_t aliveCount = 0;
	int32_t directionX = 0;
	int32_t directionY = 0;
	int32_t nextDirectionX = 0;
	double moveTimer = 0;

	// Set up all the alien data so theyre arranged in a nice big group in the middle of the screen
	void setup() {
		int32_t const groupWidth = AlienCols * AlienCellSize;
		int32_t const groupHeight = AlienRows * AlienCellSize;
		const int32_t xStart = (Engine::CanvasWidth - groupWidth) / 2;
		const int32_t yStart = Engine::CanvasHeight - groupHeight - (AlienCellSize * 5) + AlienBuffer;

		moveTimer = AlienMoveDelay;
		aliveCount = AlienCols * AlienRows;
		directionX = 1;
		directionY = 0;

		for (int i = 0; i < aliveCount; ++i) {
			aliens[i].x = xStart + (i % AlienCols) * AlienCellSize;
			aliens[i].y = yStart + (i / AlienCols) * AlienCellSize;
		}
	}

	// moves the aliens in one large step in the direction currently set
	void update(double deltaTime) {
		moveTimer -= deltaTime;
		if (moveTimer <= 0) {
			moveTimer += AlienMoveDelay;
			int32_t wallHit = 0;
			for (int i = 0; i < aliveCount; ++i) {
				aliens[i].x += directionX * AlienMoveDistamce;
				aliens[i].y += directionY * AlienMoveDistamce;
				if (directionX > 0 && (aliens[i].x + AlienCellSize) > (Engine::CanvasWidth - AlienMoveDistamce)) {
					// moving to the right and the right side of the alien will pass the canvas edge next move
					wallHit = 1;
				} else if (directionX < 0 && aliens[i].x <= AlienMoveDistamce) {
					// moving to the left and the left side of the alien will pass the canvas edge next move
					wallHit = 1;
				} else if (aliens[i].y + AlienCellSize >= Engine::CanvasHeight) {
					// the bottom edge of the alien is on or past the bottom of the canvas
					gameOver = 1;
				}
			}
			if (directionY > 0) {
				// if we just moved vertically. restart moving side to the side again
				directionX = nextDirectionX;
				directionY = 0;
			} else if (wallHit) {
				// if we hit a wall, start moving vertically down and store the next horiz direction
				nextDirectionX = directionX * -1;
				directionX = 0;
				directionY = 1;
			}
		}
	}

	// draw all alive aliens
	void draw(Engine& engine) {
		// all alive aliens are packed to the front of the alien buffer so
		// just draw all alive aliens
		for (int i = 0; i < aliveCount; ++i) {
			engine.drawSprite(Engine::Sprite::Enemy1, aliens[i].x, aliens[i].y);
		}
	}

	// removes an alien from the alien buffer, shuffling all remaining alive
	// aliens back one to keep the buffer packed.
	// it may seem like a lot of operations per alien kill but the cache should
	// eat this up.
	void kill(int32_t index) {
		aliveCount -= 1;
		for (int i = index; i < aliveCount; ++i) {
			aliens[i] = aliens[i + 1];
		}
	}
}

// while we could have multiple rockets the descision was made to only allow one rocket at
// a time for gameplay reasons. if this changes it wouldnt be hard to change this to a type.
namespace RocketData {
	int32_t x = 0;
	double y = 0; // floating point for smooth movement
	int32_t alive = 0;

	void kill();

	void update(double deltaTime) {
		if (alive) {
			y -= RocketSpeed * deltaTime;
			if (y <= -Engine::SpriteSize) {
				kill();
			} else {
				int32_t rCenterX = x + Engine::SpriteSize / 2;
				int32_t rCenterY = static_cast<int>(y) + Engine::SpriteSize / 2;
				for (int i = 0; i < AlienData::aliveCount; ++i) {
					if (rCenterX > AlienData::aliens[i].x && rCenterX < AlienData::aliens[i].x + Engine::SpriteSize &&
						rCenterY > AlienData::aliens[i].y && rCenterY < AlienData::aliens[i].y + Engine::SpriteSize) {
						AlienData::kill(i);
						kill();
						break;
					}
				}
			}
		}
	}

	void draw(Engine& engine) {
		if (alive) {
			engine.drawSprite(Engine::Sprite::Rocket, x, static_cast<int>(y));
		}
	}

	void spawn(int _x, int _y) {
		x = _x;
		y = _y;
		alive = 1;
	}

	void kill() {
		alive = 0;
	}
}

void EngineMain()
{
	Engine engine;

	AlienData::setup();

	double playerX = (Engine::CanvasWidth - Engine::SpriteSize) / 2; 
	int32_t const playerY = Engine::CanvasHeight - Engine::SpriteSize; // player cant move vertically
	double fireDelay = 0.2;

	double lastTimeStamp = engine.getStopwatchElapsedSeconds();

	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		double deltaTime = timestamp - lastTimeStamp;
		lastTimeStamp = timestamp;

		if (gameOver) {
			const char message[] = "Game Over!";
			engine.drawText(
				message, 
				(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
				(Engine::CanvasHeight - Engine::FontRowHeight) / 2);
		} else {
			if (fireDelay > 0) {
				fireDelay -= deltaTime;
			}
			
			Engine::PlayerInput keys = engine.getPlayerInput();
			if (keys.left)  playerX -= PlayerSpeed * deltaTime;
			if (keys.right) playerX += PlayerSpeed * deltaTime;
			if (fireDelay <= 0 && RocketData::alive == 0 && keys.fire) {
				RocketData::spawn(static_cast<int>(playerX), playerY);
				fireDelay = 0.2;
			}

			RocketData::update(deltaTime);

			AlienData::update(deltaTime);
		}

		engine.drawSprite(Engine::Sprite::Player, static_cast<int>(playerX), playerY);

		AlienData::draw(engine);
		RocketData::draw(engine);
	}
}

