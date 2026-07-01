#include "Camera.h"
#include "Constants.h"
#include <algorithm>

Camera::Camera(float width, float height)
    : initialSize(width, height) {
    view.setSize(initialSize);
    view.setCenter({ width / 2.0f, height / 2.0f });
}

void Camera::SetBounds(const sf::FloatRect& levelBounds) {
    bounds    = levelBounds;
    hasBounds = true;
    ApplyBounds();
}

void Camera::ApplyBounds() {
    if (!hasBounds) return;

    sf::Vector2f center  = view.getCenter();
    sf::Vector2f viewSize = view.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    if (bounds.size.x <= viewSize.x) {
        center.x = bounds.position.x + bounds.size.x / 2.0f;
    }
    else {
        center.x = std::clamp(center.x,
                              bounds.position.x + halfW,
                              bounds.position.x + bounds.size.x - halfW);
    }

    if (bounds.size.y <= viewSize.y) {
        center.y = bounds.position.y + bounds.size.y / 2.0f;
    }
    else {
        center.y = std::clamp(center.y,
                              bounds.position.y + halfH,
                              bounds.position.y + bounds.size.y - halfH);
    }

    view.setCenter(center);
}

void Camera::Update() {
    if (followTarget && !followTarget->IsDead()) {
        view.setCenter(followTarget->GetPosition());
    }
    ApplyBounds();
}

void Camera::StartPan(sf::Vector2f mousePos) {
    if (!followTarget) {
        isPanning    = true;
        lastMousePos = mousePos;
    }
}

void Camera::UpdatePan(sf::Vector2f mousePos) {
    if (!isPanning || followTarget) return;

    sf::Vector2f delta = lastMousePos - mousePos;
    view.move(delta);

    // Record where we ended up before and after clamping.
    sf::Vector2f preClamp = view.getCenter();
    ApplyBounds();
    sf::Vector2f postClamp = view.getCenter();

    // Any difference between pre- and post-clamp means the bounds absorbed some
    // of the intended pan. Feed that absorbed amount back into the anchor so the
    // anchor stays in sync with the actual camera position and the jitter stops.
    sf::Vector2f absorbed = preClamp - postClamp;
    lastMousePos = mousePos + absorbed;
}

void Camera::StopPan() {
    isPanning = false;
}

void Camera::Zoom(float delta) {
    // Positive delta (scroll up) = zoom in = smaller view size.
    const float   factor  = 1.0f - delta * Camera_Settings::ZOOM_SPEED;
    sf::Vector2f  newSize = view.getSize() * factor;

    // Clamp relative to the initial (1×) view size.
    const float scaleX = newSize.x / initialSize.x;
    if (scaleX < Camera_Settings::MIN_ZOOM)
        newSize = initialSize * Camera_Settings::MIN_ZOOM;
    else if (scaleX > Camera_Settings::MAX_ZOOM)
        newSize = initialSize * Camera_Settings::MAX_ZOOM;

    view.setSize(newSize);
    ApplyBounds();
}

void Camera::SetTarget(const GameObject* target) {
    followTarget = target;
    isPanning    = false;
}

void Camera::ClearTarget() {
    followTarget = nullptr;
}

void Camera::ResetView() {
    followTarget = nullptr;
    isPanning    = false;
    view.setSize(initialSize);
    view.setCenter({ initialSize.x / 2.0f, initialSize.y / 2.0f });
    ApplyBounds();
}

const sf::View& Camera::GetView() const {
    return view;
}
