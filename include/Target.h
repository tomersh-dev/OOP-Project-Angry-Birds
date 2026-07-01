#pragma once
#include "GameObject.h"

class Target : public GameObject {
public:
    Target(b2World* world, float x, float y, float radius, float maxHealth, const sf::Texture& texture);
    ~Target() override = default;

    void TakeDamage(float damage) override;

protected:
    float health;
};