#include "Target.h"
#include "Constants.h"

Target::Target(b2World* world, float x, float y, float radius, float maxHealth, const sf::Texture& texture)
    : GameObject(texture) {

    health = maxHealth;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / Physics::PPM, y / Physics::PPM);
    body = world->CreateBody(&bodyDef);

    b2CircleShape shape;
    shape.m_radius = radius / Physics::PPM;

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &shape;
    fixtureDef.density     = Target_Settings::DENSITY;
    fixtureDef.friction    = Target_Settings::FRICTION;
    fixtureDef.restitution = Target_Settings::RESTITUTION;
    fixtureDef.filter.categoryBits = CollisionFilter::TARGET;
    fixtureDef.filter.maskBits     = CollisionFilter::ALL;
    body->CreateFixture(&fixtureDef);

    body->SetUserData(reinterpret_cast<void*>(this));

    sf::Vector2u texSize = texture.getSize();
    sprite.setOrigin({ static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f });

    float scale = (radius * 2.0f) / static_cast<float>(texSize.x);
    sprite.setScale({ scale, scale });
}

void Target::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0.0f) {
        MarkForDestruction();
    }
}