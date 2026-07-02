#include "Slingshot.h"
#include "Constants.h"
#include <cmath>

Slingshot::Slingshot(float x, float y, float width, float height, const sf::Texture& texture)
    : position(x, y), launchPoint(x, y - height * Slingshot_Settings::FORK_HEIGHT_RATIO), sprite(texture) {

    sf::Vector2u texSize = texture.getSize();
    sprite.setOrigin({ static_cast<float>(texSize.x) / 2.0f, static_cast<float>(texSize.y) / 2.0f });
    sprite.setScale({ width / static_cast<float>(texSize.x), height / static_cast<float>(texSize.y) });
    sprite.setPosition(position);
}

void Slingshot::AttachProjectile(Projectile* projectile) {
    currentProjectile = projectile;
    firedProjectile = nullptr;
    isPulling = false;

    currentProjectile->SetGravityScale(0.0f);   // hold the projectile in place
    currentProjectile->SetLinearVelocity({ 0.0f, 0.0f });
    currentProjectile->SetPosition(launchPoint);
}

void Slingshot::Update(const sf::Vector2f& mousePos, bool isLeftClicked) {
    if (!currentProjectile) return;

    if (isLeftClicked) {
        if (!isPulling) {
            sf::Vector2f projPos = currentProjectile->GetPosition();
            float dx = mousePos.x - projPos.x;
            float dy = mousePos.y - projPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist <= Slingshot_Settings::GRAB_RADIUS) {
                isPulling = true;
            }
        }

        if (isPulling) {
            sf::Vector2f pullVector = mousePos - launchPoint;
            float distance = std::sqrt(pullVector.x * pullVector.x + pullVector.y * pullVector.y);

            if (distance > Slingshot_Settings::MAX_PULL_DISTANCE) {
                pullVector = (pullVector / distance) * Slingshot_Settings::MAX_PULL_DISTANCE;
            }

            currentProjectile->SetPosition(launchPoint + pullVector);
        }
    }
    else if (isPulling) {
        Fire();
    }
}

void Slingshot::Fire() {
    isPulling = false;

    sf::Vector2f projPos = currentProjectile->GetPosition();
    sf::Vector2f impulseVector = launchPoint - projPos;
    impulseVector.x *= Slingshot_Settings::POWER_MULTIPLIER;
    impulseVector.y *= Slingshot_Settings::POWER_MULTIPLIER;

    currentProjectile->SetGravityScale(1.0f);    // restore gravity before launch
    currentProjectile->SetAwake(true);
    currentProjectile->ApplyLinearImpulse(impulseVector);

    firedProjectile = currentProjectile;
    currentProjectile = nullptr;
}

void Slingshot::Render(sf::RenderWindow& window) const {
    window.draw(sprite);

    if (!isPulling || !currentProjectile) return;

    sf::Vector2f projPos = currentProjectile->GetPosition();

    // Rubber band
    sf::Vertex rubberBand[] = {
        sf::Vertex(launchPoint, sf::Color::Black),
        sf::Vertex(projPos, sf::Color::Black)
    };
    window.draw(rubberBand, 2, sf::PrimitiveType::Lines);

    // Trajectory prediction
    // Replicate the impulse Fire() would apply, then derive initial velocity.
    sf::Vector2f impulse = launchPoint - projPos;
    impulse.x *= Slingshot_Settings::POWER_MULTIPLIER;
    impulse.y *= Slingshot_Settings::POWER_MULTIPLIER;

    // v0 = J / m  (Box2D units: J in N·s, m in kg → v0 in m/s)
    const float mass = currentProjectile->GetMass();
    const float v0x  = impulse.x / mass;
    const float v0y  = impulse.y / mass;

    // P0 in Box2D metres
    const float p0x = projPos.x / Physics::PPM;
    const float p0y = projPos.y / Physics::PPM;

    for (int i = 0; i < Trajectory::NUM_DOTS; ++i) {
        const float t = (static_cast<float>(i) + 1.0f) * Trajectory::TIME_STEP;

        // Kinematic: P(t) = P0 + v0·t + ½·g·t²
        const float px = (p0x + v0x * t) * Physics::PPM;
        const float py = (p0y + v0y * t + 0.5f * Physics::GRAVITY * t * t) * Physics::PPM;

        // Dots shrink and fade with distance
        const float frac   = static_cast<float>(i) / static_cast<float>(Trajectory::NUM_DOTS);
        const float radius = Trajectory::MAX_DOT_RADIUS * (1.0f - frac);
        const auto  alpha  = static_cast<std::uint8_t>(255u * (1.0f - frac));

        sf::CircleShape dot(radius);
        dot.setOrigin({ radius, radius });
        dot.setFillColor(sf::Color(255, 255, 200, alpha));
        dot.setPosition({ px, py });
        window.draw(dot);
    }
}

Projectile* Slingshot::GetFiredProjectile() const {
    return firedProjectile;
}

void Slingshot::ClearFiredProjectile() {
    firedProjectile = nullptr;
}
