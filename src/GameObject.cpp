#include "GameObject.h"
#include "Constants.h"

GameObject::GameObject(const sf::Texture& texture) : sprite(texture) {
}

void GameObject::Update() {
    if (!body || isDead) return;

    b2Vec2 pos = body->GetPosition();
    sprite.setPosition({ pos.x * Physics::PPM, pos.y * Physics::PPM });

    sprite.setRotation(sf::radians(body->GetAngle()));
}

void GameObject::Render(sf::RenderWindow& window) {
    if (!isDead) {
        window.draw(sprite);
    }
}

void GameObject::TakeDamage(float damage) {
}

void GameObject::MarkForDestruction() {
    isDead = true;
}

bool GameObject::IsDead() const {
    return isDead;
}

void GameObject::ApplyLinearImpulse(sf::Vector2f impulse) {
    if (body) {
        body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
    }
}

void GameObject::SetLinearVelocity(sf::Vector2f velocity) {
    if (body) {
        body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
    }
}

sf::Vector2f GameObject::GetLinearVelocity() const {
    if (body) {
        b2Vec2 vel = body->GetLinearVelocity();
        return { vel.x, vel.y };
    }
    return { 0.0f, 0.0f };
}

void GameObject::SetGravityScale(float scale) {
    if (body) body->SetGravityScale(scale);
}

void GameObject::SetAwake(bool awake) {
    if (body) body->SetAwake(awake);
}

void GameObject::DestroyPhysics(b2World* world) {
    if (body) {
        world->DestroyBody(body);
        body = nullptr;
    }
}

sf::Vector2f GameObject::GetPosition() const {
    return sprite.getPosition();
}

void GameObject::SetPosition(sf::Vector2f newPos) {
    if (body) {
        body->SetTransform(b2Vec2(newPos.x / Physics::PPM, newPos.y / Physics::PPM), body->GetAngle());
    }
    sprite.setPosition(newPos);
}