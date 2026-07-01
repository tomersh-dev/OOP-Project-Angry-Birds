#pragma once
#include "Projectile.h"

class BoostProjectile : public Projectile {
public:
    BoostProjectile(b2World* world, float x, float y, float radius, const sf::Texture& texture);
    ~BoostProjectile() override = default;

    void ActivateAbility() override;
};
