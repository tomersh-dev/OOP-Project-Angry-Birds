#include "ObjectFactory.h"
#include "Constants.h"

Block* ObjectFactory::CreateBlock(std::istringstream& iss, const std::string& blockType, b2World& world) {
    float x, y, width, height;
    iss >> x >> y >> width >> height;

    AssetManager& assets = AssetManager::GetInstance();

    if (blockType == "ICE")   return new IceBlock  (&world, x, y, width, height, assets.GetTexture("ice"));
    if (blockType == "STONE") return new StoneBlock (&world, x, y, width, height, assets.GetTexture("stone"));
    return                           new WoodBlock  (&world, x, y, width, height, assets.GetTexture("wood"));
}

Target* ObjectFactory::CreateTarget(std::istringstream& iss, b2World& world) {
    float x, y, radius, health;
    iss >> x >> y >> radius >> health;
    return new Target(&world, x, y, radius, health, AssetManager::GetInstance().GetTexture("pig"));
}

Projectile* ObjectFactory::CreateProjectile(const std::string& typeStr, float x, float y, float radius, b2World& world) {
    AssetManager& assets = AssetManager::GetInstance();

    if (typeStr == "BOOST")
        return new BoostProjectile (&world, x, y, radius, assets.GetTexture("boost_projectile"));

    return     new NormalProjectile(&world, x, y, radius, assets.GetTexture("normal_projectile"));
}
