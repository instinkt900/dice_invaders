#include "Rocket.h"
#include "GameContext.h"
#include "GameConstants.h"

void Rocket::update(float deltaTime) {
	if (alive) {
		y -= RocketSpeed * deltaTime;
		if (y <= -Engine::SpriteSize) {
			// top edge of the canvas passed
			kill();
		} else {
			// just use the center point of the rocket for collision detection.
			int32_t const rCenterX = x + Engine::SpriteSize / 2;
			int32_t const rCenterY = static_cast<int>(y) + Engine::SpriteSize / 2;
			// translate its position relative to the alien group
			int32_t const groupX = g_context.alienGroup.aliens[0].x;
			int32_t const groupY = g_context.alienGroup.aliens[0].y;
			// scale the position to the group cell coordinate space
			int32_t const cellX = (rCenterX - groupX) / AlienCellSize;
			int32_t const cellY = (rCenterY - groupY) / AlienCellSize;
			// test that we're within the groups bounds
			if (cellX >= 0 && cellX < AlienCols && cellY >= 0 && cellY < AlienRows) {
				// get the position relative to the cell we're in
				int32_t const innerCellX = (rCenterX - groupX) % AlienCellSize;
				int32_t const innerCellY = (rCenterY - groupY) % AlienCellSize;
				// test that we're not in the "buffer zone" around the alien
				if (innerCellX > AlienBuffer && innerCellX < AlienCellSize - AlienBuffer
					&& innerCellY > AlienBuffer && innerCellY < AlienCellSize - AlienBuffer) {
						// by here we've collided with an alien cell
						int32_t const alienIndex = cellX + cellY * AlienCols;
						if (g_context.alienGroup.aliens[alienIndex].alive) {
							// it was alive. not anymore.
							g_context.alienGroup.kill(alienIndex);
							kill();
							g_context.player.addScore(ScorePerAlien);
						}
				}
			}
		}
	}
}

void Rocket::draw() {
	if (alive) {
		g_context.entityBuffer.addEntity(Engine::Sprite::Rocket, x, static_cast<int32_t>(y));
	}
}

void Rocket::spawn(int32_t _x, int32_t _y) {
	x = _x;
	y = _y;
	alive = true;
}

void Rocket::kill() {
	alive = false;
}
