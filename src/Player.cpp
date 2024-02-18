#include "Player.h"
#include "GameContext.h"
#include <stdio.h>

void Player::setup() {
	x = (Engine::CanvasWidth - Engine::SpriteSize) / 2; 
	y = Engine::CanvasHeight - Engine::SpriteSize;
	health = PlayerHealth;
	fireDelay = PlayerFireDelay;
	score = 0;
	updateScore();
}

void Player::update(float deltaTime, Engine& engine) {
	if (fireDelay > 0) {
		fireDelay -= deltaTime;
	}
	
	Engine::PlayerInput const keys = engine.getPlayerInput();
	if (keys.left) {
		x = std::max(x - PlayerSpeed * deltaTime, -Engine::SpriteSize / 2.0f); // clamp the player position to the canvas
	}
	if (keys.right) {
		x = std::min(x + PlayerSpeed * deltaTime, static_cast<float>(Engine::CanvasWidth - Engine::SpriteSize / 2)); // clamp the player position to the canvas
	}
	if (keys.fire && fireDelay <= 0 && g_context.rocket.alive == 0) {
		g_context.rocket.spawn(static_cast<int32_t>(x), y);
		fireDelay = PlayerFireDelay;
	}
}

void Player::draw() {
	g_context.entityBuffer.addEntity(Engine::Sprite::Player, static_cast<int32_t>(x), y);
}

void Player::addScore(int32_t amount) {
	score += amount;
	updateScore();
}

void Player::updateScore() {
	snprintf(scoreBuffer, ScoreBufferSize - 1, "Score: %d", score);
}

void Player::hit() {
	--health;
	if (health == 0) {
		g_context.gameState = GameState::GameOver;
	}
}
