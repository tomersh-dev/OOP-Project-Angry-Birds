#pragma once
#include "GameObject.h"

class Block : public GameObject {
public:
    Block(b2World* world, float x, float y, float width, float height,
          float density, float friction, float restitution,
          float maxHealth, const sf::Texture& texture);
    ~Block() override = default;

    void TakeDamage(float damage) override;
    virtual int GetScoreValue() const = 0;

protected:
    float health;
};
