#pragma once

#include "GameConstants.h"
#include <array>

// the idea of this is that as we're updating and drawing entities in the game, we just
// batch them here and then before moving onto the next game step we "flush" this buffer
// and just submit all the entities to be rendered at once.
struct EntityBuffer {
	struct Entity {
		int32_t x;
		int32_t y;
		Engine::Sprite sprite;
	};

	void addEntity(Engine::Sprite sprite, int32_t x, int32_t y);
	void flush(Engine& engine);

    std::array<Entity, MaxEntities> entities;
    int32_t entityCount = 0;
};
