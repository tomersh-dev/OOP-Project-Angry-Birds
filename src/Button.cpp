#include "Button.h"

const sf::Color Button::COLOR_NORMAL  {  50,  80, 130, 220 };
const sf::Color Button::COLOR_HOVER   {  90, 130, 200, 255 };
const sf::Color Button::COLOR_OUTLINE {  30,  50, 100, 255 };

Button::Button(sf::Vector2f position, sf::Vector2f size,
               const std::string& label, const sf::Font& font, unsigned int fontSize)
    : text(font, label, fontSize) {

    shape.setPosition(position);
    shape.setSize(size);
    shape.setFillColor(COLOR_NORMAL);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(COLOR_OUTLINE);

    // Center the text within the button rectangle
    sf::FloatRect tb = text.getLocalBounds();
    text.setOrigin({ tb.position.x + tb.size.x / 2.0f,
                     tb.position.y + tb.size.y / 2.0f });
    text.setPosition({ position.x + size.x / 2.0f,
                       position.y + size.y / 2.0f });
    text.setFillColor(sf::Color::White);
}

void Button::Update(const sf::Vector2f& mousePos) {
    shape.setFillColor(
        shape.getGlobalBounds().contains(mousePos) ? COLOR_HOVER : COLOR_NORMAL);
}

bool Button::IsClicked(const sf::Vector2f& mousePos, bool isLeftClicked) const {
    return isLeftClicked && shape.getGlobalBounds().contains(mousePos);
}

void Button::Render(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}
