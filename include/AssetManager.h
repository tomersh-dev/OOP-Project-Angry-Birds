#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
public:
    static AssetManager& GetInstance();

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    void LoadTexture(const std::string& name, const std::string& filename);
    const sf::Texture& GetTexture(const std::string& name) const;

private:
    AssetManager() = default;
    ~AssetManager() = default;

    std::map<std::string, sf::Texture> textures;
};
