#include "Projectile.h"
#include "Constants.h"

Projectile::Projectile(b2World* world, float x, float y, float radius, const sf::Texture& texture)
    : GameObject(texture) {

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / Physics::PPM, y / Physics::PPM);
    bodyDef.bullet = true;
    body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius / Physics::PPM;

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &shape;
    fixtureDef.density     = Projectile_Settings::DENSITY;
    fixtureDef.friction    = Projectile_Settings::FRICTION;
    fixtureDef.restitution = Projectile_Settings::RESTITUTION;
    fixtureDef.filter.categoryBits = CollisionFilter::PROJECTILE;
    fixtureDef.filter.maskBits     = CollisionFilter::BLOCK
                                   | CollisionFilter::TARGET
                                   | CollisionFilter::GROUND;
    body->CreateFixture(&fixtureDef);

    body->SetUserData(reinterpret_cast<void*>(this));

    sf::Vector2u texSize = texture.getSize();
    sprite.setOrigin({ static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f });

    float scale = (radius * 2.0f) / static_cast<float>(texSize.x);
    sprite.setScale({ scale, scale });
}

bool Projectile::HasAbilityBeenUsed() const {
    return abilityUsed;
}

float Projectile::GetMass() const {
    return body ? body->GetMass() : 1.0f;
}
