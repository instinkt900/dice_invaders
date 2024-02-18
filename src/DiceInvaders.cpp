#include "Engine.h"
#include "GameContext.h"

#include <stdio.h>

GameContext g_context;

/**
 * Some overall notes.
 * 
 * I'm preferring structs over classes here simply for simplicity since structs default to public members. While
 * private members do allow some amount of safety by hiding away specific internals, for projects like this I think
 * the ease of development makes up for any benefit of hiding internals away.
 * 
 * I am also ignoring constructors completely since initializers can be used on members, but also we are rarely
 * constructing anything. Most instances of objects are just reused dead objects. I also prefer to use member
 * initializers to set an initial state for simplicity and constructors tend to lead to complex behaviour issues,
 * ie. accidentally accessing members that havent been initialized yet, calling methods before the object is
 * ready, possible exceptions, etc.
 */

// a helper for drawing centered text
// Should only call it with static constant "strings like this" otherwise
// the length might be wrong.
template<std::size_t Length>
constexpr void drawCenteredText(Engine& engine, const char(&message)[Length]) {
	engine.drawText(message,
		(Engine::CanvasWidth - (static_cast<int>(Length) - 1) * Engine::FontWidth) / 2,
		(Engine::CanvasHeight - Engine::FontRowHeight) / 2);
}

void resetGame() {
	g_context.player.setup();
	g_context.alienGroup.setup();
	g_context.rocket.kill();
}

void gameLoop(Engine& engine) {
	double lastTimeStamp = engine.getStopwatchElapsedSeconds();
	while (engine.startFrame()) {
		double const timestamp = engine.getStopwatchElapsedSeconds();
		float const deltaTime = static_cast<float>(timestamp - lastTimeStamp);
		lastTimeStamp = timestamp;

		engine.drawText(g_context.player.scoreBuffer, 15, 5);

		if (g_context.gameState == GameState::Game) {
			g_context.player.update(deltaTime, engine);
			g_context.alienGroup.update(deltaTime);
			g_context.rocket.update(deltaTime);
		}

		g_context.player.draw();
		g_context.alienGroup.draw();
		g_context.rocket.draw();

		g_context.entityBuffer.flush(engine);
		for (int32_t i = 0; i < g_context.player.health; ++i) {
			engine.drawSprite(Engine::Sprite::Player, Engine::CanvasWidth - 10 - Engine::SpriteSize * (PlayerHealth - i), 5);
		}

		if (g_context.gameState == GameState::GameOver) {
			drawCenteredText(engine, "Game Over");
			Engine::PlayerInput const keys = engine.getPlayerInput();
			if (keys.fire) {
				resetGame();
				g_context.gameState = GameState::Game;
			}
		}
	}
}

void EngineMain()
{
	Engine engine;

	resetGame();
	g_context.gameState = GameState::Intro;

	while (engine.startFrame())
	{
		switch (g_context.gameState) {
			case GameState::Intro: {
				drawCenteredText(engine, "Press space to begin.");
				Engine::PlayerInput const keys = engine.getPlayerInput();
				if (keys.fire) {
					g_context.gameState = GameState::Game;
				}
			}
				break;
			case GameState::Game:
				gameLoop(engine);
				return; // when we exit out of gameloop we exit the game
			case GameState::GameOver:
				// should never get here
				break;
		}
	}
}
