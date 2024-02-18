#include "EntityBuffer.h"

void EntityBuffer::addEntity(Engine::Sprite sprite, int32_t x, int32_t y) {
    if (entityCount < MaxEntities) {
        entities[entityCount] = { x, y, sprite };
        ++entityCount;
    }
}

void EntityBuffer::flush(Engine& engine) {
    for (int32_t i = 0; i < entityCount; ++i) {
        engine.drawSprite(entities[i].sprite, entities[i].x, entities[i].y);
    }
    entityCount = 0;
}
