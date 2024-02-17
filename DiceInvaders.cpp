#include "Engine.h"

void EngineMain()
{
	Engine engine;

	int x = (Engine::CanvasWidth - Engine::SpriteSize) / 2; 

	while (engine.startFrame())
	{
		double timestamp = engine.getStopwatchElapsedSeconds();
		(void)timestamp;
		
		Engine::PlayerInput keys = engine.getPlayerInput();
		if (keys.left)  --x;
		if (keys.right) ++x;

		engine.drawSprite(
			Engine::Sprite::Player, 
			x, Engine::CanvasHeight - Engine::SpriteSize);

		const char message[] = "Good Luck!";
		engine.drawText(
			message, 
			(Engine::CanvasWidth - (sizeof(message) - 1) * Engine::FontWidth) / 2, 
			(Engine::CanvasHeight - Engine::FontRowHeight) / 2); 
	}
}

