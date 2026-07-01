#pragma once
#include "GameObject.h"

class Projectile : public GameObject {
public:
    Projectile(b2World* world, float x, float y, float radius, const sf::Texture& texture);
    ~Projectile() override = default;

    virtual void ActivateAbility() = 0;

    bool  HasAbilityBeenUsed() const;
    float GetMass() const;

protected:
    bool abilityUsed = false;
};
