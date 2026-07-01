#include "IceBlock.h"
#include "Constants.h"

IceBlock::IceBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture)
    : Block(world, x, y, width, height,
            Materials::Ice::DENSITY, Materials::Ice::FRICTION, Materials::Ice::RESTITUTION,
            Materials::Ice::HEALTH, texture) {}

int IceBlock::GetScoreValue() const {
    return Materials::Ice::SCORE;
}
