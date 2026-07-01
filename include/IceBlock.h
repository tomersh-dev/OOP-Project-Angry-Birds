#pragma once
#include "Block.h"

class IceBlock : public Block {
public:
    IceBlock(b2World* world, float x, float y, float width, float height, const sf::Texture& texture);
    ~IceBlock() override = default;

    int GetScoreValue() const override;
};
