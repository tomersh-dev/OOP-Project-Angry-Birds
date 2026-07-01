#include "Block.h"
#include "Constants.h"

Block::Block(b2World* world, float x, float y, float width, float height,
             float density, float friction, float restitution,
             float maxHealth, const sf::Texture& texture)
    : GameObject(texture) {

    health = maxHealth;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x / Physics::PPM, y / Physics::PPM);
    body = world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox((width / 2.0f) / Physics::PPM, (height / 2.0f) / Physics::PPM);

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &shape;
    fixtureDef.density     = density;
    fixtureDef.friction    = friction;
    fixtureDef.restitution = restitution;
    fixtureDef.filter.categoryBits = CollisionFilter::BLOCK;
    fixtureDef.filter.maskBits     = CollisionFilter::ALL;
    body->CreateFixture(&fixtureDef);

    body->SetUserData(reinterpret_cast<void*>(this));

    sf::Vector2u texSize = texture.getSize();
    sprite.setOrigin({ static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f });
    sprite.setScale({ width / static_cast<float>(texSize.x), height / static_cast<float>(texSize.y) });
}

void Block::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0.0f) {
        MarkForDestruction();
    }
}
