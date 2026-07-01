#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button(sf::Vector2f position, sf::Vector2f size,
           const std::string& label, const sf::Font& font, unsigned int fontSize);

    void Update(const sf::Vector2f& mousePos);
    bool IsClicked(const sf::Vector2f& mousePos, bool isLeftClicked) const;
    void Render(sf::RenderWindow& window) const;

private:
    sf::RectangleShape shape;
    sf::Text           text;

    static const sf::Color COLOR_NORMAL;
    static const sf::Color COLOR_HOVER;
    static const sf::Color COLOR_OUTLINE;
};
