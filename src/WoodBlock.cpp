#include "WoodBlock.h"
#include "Constants.h"

WoodBlock::WoodBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture)
    : Block(world, x, y, width, height,
            Materials::Wood::DENSITY, Materials::Wood::FRICTION, Materials::Wood::RESTITUTION,
            Materials::Wood::HEALTH, texture) {}

int WoodBlock::GetScoreValue() const {
    return Materials::Wood::SCORE;
}
