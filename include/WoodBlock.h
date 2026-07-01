#pragma once
#include "Block.h"

class WoodBlock : public Block {
public:
    WoodBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture);
    ~WoodBlock() override = default;

    int GetScoreValue() const override;
};
