#include "ContactListener.h"
#include "Constants.h"
#include <algorithm>

void ContactListener::SetDamageEnabled(bool enabled) {
    isDamageEnabled = enabled;
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    if (!isDamageEnabled) return;

    float maxImpulse = 0.0f;
    for (int32 i = 0; i < impulse->count; ++i) {
        maxImpulse = std::max(maxImpulse, impulse->normalImpulses[i]);
    }

    if (maxImpulse < Physics::DAMAGE_THRESHOLD) return;

    void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
    void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();

    if (userDataA) reinterpret_cast<GameObject*>(userDataA)->TakeDamage(maxImpulse);
    if (userDataB) reinterpret_cast<GameObject*>(userDataB)->TakeDamage(maxImpulse);
}
