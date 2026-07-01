#pragma once
#include <box2d/box2d.h>
#include "GameObject.h"

class ContactListener : public b2ContactListener {
public:
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

    void SetDamageEnabled(bool enabled);

private:
    bool isDamageEnabled = false;
};
