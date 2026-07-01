#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Camera {
public:
    Camera(float width, float height);
    ~Camera() = default;

    void Update();

    void StartPan(sf::Vector2f mousePos);
    void UpdatePan(sf::Vector2f mousePos);
    void StopPan();

    void Zoom(float delta);

    void SetTarget(const GameObject* target);
    void ClearTarget();
    void ResetView();

    void SetBounds(const sf::FloatRect& levelBounds);

    const sf::View& GetView() const;

private:
    void ApplyBounds();

    sf::View     view;
    sf::Vector2f initialSize;       // view size at 1× zoom, used for clamping

    const GameObject* followTarget = nullptr;
    bool         isPanning   = false;
    sf::Vector2f lastMousePos;

    sf::FloatRect bounds;
    bool          hasBounds  = false;
};
