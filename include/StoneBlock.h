#pragma once
#include "Block.h"

class StoneBlock : public Block {
public:
    StoneBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture);
    ~StoneBlock() override = default;

    int GetScoreValue() const override;
};
