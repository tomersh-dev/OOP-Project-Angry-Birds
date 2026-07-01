#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class GameObject {
public:
    GameObject(const sf::Texture& texture);
    virtual ~GameObject() = default;

    virtual void Update();
    virtual void Render(sf::RenderWindow& window);
    virtual void TakeDamage(float damage);
    virtual void MarkForDestruction();

    bool IsDead() const;

    void ApplyLinearImpulse(sf::Vector2f impulse);
    void SetLinearVelocity(sf::Vector2f velocity);
    sf::Vector2f GetLinearVelocity() const;

    void SetGravityScale(float scale);
    void SetAwake(bool awake);

    void DestroyPhysics(b2World* world);

    sf::Vector2f GetPosition() const;

    void SetPosition(sf::Vector2f newPos);

protected:
    b2Body* body = nullptr;
    sf::Sprite sprite;
    bool isDead = false;
};