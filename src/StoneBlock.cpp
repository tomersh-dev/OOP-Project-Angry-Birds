#include "StoneBlock.h"
#include "Constants.h"

StoneBlock::StoneBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture)
    : Block(world, x, y, width, height,
            Materials::Stone::DENSITY, Materials::Stone::FRICTION, Materials::Stone::RESTITUTION,
            Materials::Stone::HEALTH, texture) {}

int StoneBlock::GetScoreValue() const {
    return Materials::Stone::SCORE;
}
