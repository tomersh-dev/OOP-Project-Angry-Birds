#pragma once
#include <string>
#include <sstream>
#include <box2d/box2d.h>
#include "Block.h"
#include "WoodBlock.h"
#include "IceBlock.h"
#include "StoneBlock.h"
#include "Target.h"
#include "Projectile.h"
#include "NormalProjectile.h"
#include "BoostProjectile.h"
#include "AssetManager.h"

class ObjectFactory {
public:
    static Block*      CreateBlock     (std::istringstream& iss, const std::string& blockType, b2World& world);
    static Target*     CreateTarget    (std::istringstream& iss, b2World& world);
    static Projectile* CreateProjectile(const std::string& typeStr, float x, float y, float radius, b2World& world);
};
