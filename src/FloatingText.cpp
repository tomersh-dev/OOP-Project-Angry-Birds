#include "FloatingText.h"
#include "Constants.h"
#include <algorithm>

FloatingText::FloatingText(const sf::Font& font, const std::string& str,
                           sf::Vector2f worldPos, sf::Vector2f vel, float lifespan)
    : text(font, str, FloatingText_Settings::FONT_SIZE)
    , velocity(vel)
    , timer(0.0f)
    , maxLifespan(lifespan)
{
    text.setFillColor(sf::Color(255, 220, 0, 255));
    text.setOutlineColor(sf::Color(0, 0, 0, 255));
    text.setOutlineThickness(2.0f);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.0f,
                     bounds.position.y + bounds.size.y / 2.0f });
    text.setPosition(worldPos);
}

void FloatingText::Update(float dt) {
    timer += dt;
    text.move(velocity * dt);

    float fraction = timer / maxLifespan;
    auto alpha = static_cast<uint8_t>(255u * std::max(0.0f, 1.0f - fraction));

    sf::Color fill = text.getFillColor();
    fill.a = alpha;
    text.setFillColor(fill);

    sf::Color outline = text.getOutlineColor();
    outline.a = alpha;
    text.setOutlineColor(outline);
}

bool FloatingText::IsExpired() const {
    return timer >= maxLifespan;
}

void FloatingText::Render(sf::RenderWindow& window) const {
    window.draw(text);
}
