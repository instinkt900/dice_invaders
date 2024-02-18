#include "AlienGroup.h"
#include "GameContext.h"

// Set up all the alien data so theyre arranged in a nice big group in the middle of the screen
void AlienGroup::setup() {
	// size of the full group
	int32_t const groupWidth = AlienCols * AlienCellSize;
	int32_t const groupHeight = AlienRows * AlienCellSize;
	// center x position and a y position 
	int32_t const xStart = (Engine::CanvasWidth - groupWidth) / 2;
	int32_t const yStart = Engine::CanvasHeight - groupHeight - (AlienCellSize * 5) + AlienBuffer;

	// set some basic state
	moveTimer = AlienMoveDelayMax;
	fireDelay = AlienFireDelayMax;
	resetTimer = ResetDelay;
	aliveCount = AlienCols * AlienRows;
	directionX = 1;
	directionY = 0;

	// position all the aliens
	for (int i = 0; i < aliens.size(); ++i) {
		aliens[i].x = xStart + (i % AlienCols) * AlienCellSize;
		aliens[i].y = yStart + (i / AlienCols) * AlienCellSize;
		aliens[i].alive = true;
	}
}

void AlienGroup::update(float deltaTime) {
	// if all aliens are dead, we just reset the group after a delay.
	// could add to some game difficulty here as this could cound
	// as a "new level"
	if (aliveCount == 0) {
		resetTimer -= deltaTime;
		if (resetTimer <= 0) {
			setup();
		}
	}

	updateAliens(deltaTime);
	updateBombs(deltaTime);
	updateFiring(deltaTime);
}

void AlienGroup::updateAliens(float deltaTime) {
	// we want to move the aliens in a stepped motion so we use a timer to delay each movement
	moveTimer -= deltaTime;
	if (moveTimer <= 0) {
		moveTimer += AlienMoveDelayMin + (aliveCount / static_cast<float>(MaxAliens)) * (AlienMoveDelayMax - AlienMoveDelayMin);
		int32_t wallHit = 0;
		for (int32_t i = 0; i < aliens.size(); ++i) {
			// keep updating the position of dead aliens since we rely on the [0,0] alien for rocket collision
			// could only limit this to the first alien but cache should make short work of this anyway
			aliens[i].x += directionX * AlienMoveDistance;
			aliens[i].y += directionY * AlienMoveDistance;
			if (!aliens[i].alive) {
				// dont do level collision/firing for dead enemies
				continue;
			}
			if (directionX > 0 && (aliens[i].x + AlienCellSize) > (Engine::CanvasWidth - AlienMoveDistance)) {
				// moving to the right and the right side of the alien will pass the canvas edge next move
				wallHit = 1;
			} else if (directionX < 0 && aliens[i].x <= AlienMoveDistance) {
				// moving to the left and the left side of the alien will pass the canvas edge next move
				wallHit = 1;
			} else if (aliens[i].y + AlienCellSize >= Engine::CanvasHeight) {
				// the bottom edge of the alien is on or past the bottom of the canvas
				g_context.gameState = GameState::GameOver;
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

void AlienGroup::updateFiring(float deltaTime) {
	// for firing we'll just use the timer and pick a random alien in the group as the
	// one that fired. if it's already dead then do nothing
	fireDelay -= deltaTime;
	if (fireDelay <= 0) {
		fireDelay += AlienFireDelayMin + (aliveCount / static_cast<float>(MaxAliens)) * (AlienFireDelayMax - AlienFireDelayMin);
		int32_t const randIndex = rand() % MaxAliens;
		// here we check if the random alien selected is alive, and if not, we try to next one and so on.
		// this results in a bias where the first alien after a large streak of dead aliens gets a huge
		// buff to fire rate, but its simple enough for now.
		for (int32_t i = 0; i < aliens.size(); ++i) {
			int32_t const checkIndex = (randIndex + i) % aliens.size();
			if (aliens[checkIndex].alive) {
				spawnBomb(aliens[checkIndex].x, aliens[checkIndex].y + Engine::SpriteSize); // spawn it just below the alien
				break;
			}
		}
	}
}

// bombs are just a tightly packed array of bomb structures. no fancy list or dynamic
// array/vector. we just add new bombs to a free spot after all the living bombs and
// when we remove one we just take the last bomb and put it in its place.

void AlienGroup::spawnBomb(int32_t x, int32_t y) {
	if (bombCount < MaxBombs) {
		bombs[bombCount] = { x, static_cast<float>(y) };
		++bombCount;
	}
}

void AlienGroup::updateBombs(float deltaTime) {
	for (int32_t i = 0; i < bombCount; ++i) {
		bombs[i].y += BombSpeed * deltaTime;
		if (bombs[i].y >= Engine::CanvasHeight) {
			// fell off the bottom
			destroyBomb(i);
			--i;
		} else {
			// collision detect with the player
			int32_t const centerX = bombs[i].x + Engine::SpriteSize / 2;
			int32_t const centerY = static_cast<int32_t>(bombs[i].y) + Engine::SpriteSize / 2;
			if (centerX > g_context.player.x && centerX < g_context.player.x + Engine::SpriteSize
				&& centerY > g_context.player.y && centerY < g_context.player.y + Engine::SpriteSize) {
					destroyBomb(i);
					g_context.player.hit();
					--i;
				}
		}
	}
}

void AlienGroup::destroyBomb(int32_t index) {
	// if there are bombs after this one, we just shift the last one into its place
	--bombCount;
	if (index != bombCount) {
		bombs[index] = bombs[bombCount];
	}
}

// draw all alive aliens and bombs
void AlienGroup::draw() {
	for (int32_t i = 0; i < aliens.size(); ++i) {
		if (aliens[i].alive) {
			g_context.entityBuffer.addEntity(Engine::Sprite::Enemy1, aliens[i].x, aliens[i].y);
		}
	}

	// all living bombs are in the front of the bomb array
	for (int32_t i = 0; i < bombCount; ++i) {
		g_context.entityBuffer.addEntity(Engine::Sprite::Bomb, bombs[i].x, static_cast<int32_t>(bombs[i].y));
	}
}

void AlienGroup::kill(int32_t index) {
	aliveCount -= 1;
	aliens[index].alive = false;
}
