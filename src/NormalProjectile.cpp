#include "NormalProjectile.h"

NormalProjectile::NormalProjectile(b2World* world, float x, float y, float radius, const sf::Texture& texture)
    : Projectile(world, x, y, radius, texture) {}

void NormalProjectile::ActivateAbility() {
    // No special ability
}
