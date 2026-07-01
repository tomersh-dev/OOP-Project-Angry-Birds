#include "BoostProjectile.h"
#include "Constants.h"
#include <cmath>

BoostProjectile::BoostProjectile(b2World* world, float x, float y, float radius, const sf::Texture& texture)
    : Projectile(world, x, y, radius, texture) {}

void BoostProjectile::ActivateAbility() {
    if (abilityUsed || !body) return;
    abilityUsed = true;

    b2Vec2 currentVel = body->GetLinearVelocity();
    float dirX = (currentVel.x != 0.0f) ? (currentVel.x / std::abs(currentVel.x)) : 1.0f;
    body->ApplyLinearImpulseToCenter(b2Vec2(dirX * Projectile_Settings::BOOST_IMPULSE, 0.0f), true);
}
