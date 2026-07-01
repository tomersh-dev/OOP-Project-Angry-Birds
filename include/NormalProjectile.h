#pragma once
#include "Projectile.h"

class NormalProjectile : public Projectile {
public:
    NormalProjectile(b2World* world, float x, float y, float radius, const sf::Texture& texture);
    ~NormalProjectile() override = default;

    void ActivateAbility() override;
};
