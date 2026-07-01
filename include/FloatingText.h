#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class FloatingText {
public:
    FloatingText(const sf::Font& font, const std::string& str,
                 sf::Vector2f worldPos, sf::Vector2f velocity, float lifespan);

    void Update(float dt);
    bool IsExpired() const;
    void Render(sf::RenderWindow& window) const;

private:
    sf::Text       text;
    sf::Vector2f   velocity;
    float          timer;
    float          maxLifespan;
};
