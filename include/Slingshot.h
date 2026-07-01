#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"

class Slingshot {
public:
    Slingshot(float x, float y, float width, float height, const sf::Texture& texture);
    ~Slingshot() = default;

    void Update(const sf::Vector2f& mousePos, bool isLeftClicked);
    void Render(sf::RenderWindow& window) const;

    void AttachProjectile(Projectile* projectile);
    Projectile* GetFiredProjectile() const;
    void ClearFiredProjectile();

    bool HasActiveProjectile() const;

private:
    void Fire();

    sf::Vector2f position;    // sprite centre (for drawing only)
    sf::Vector2f launchPoint; // fork tip — where the projectile sits and rubber band anchors
    sf::Sprite sprite;

    Projectile* currentProjectile = nullptr;
    Projectile* firedProjectile = nullptr;

    bool isPulling = false;
};
