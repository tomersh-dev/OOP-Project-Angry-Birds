#include "AssetManager.h"
#include <stdexcept>

AssetManager& AssetManager::GetInstance() {
    static AssetManager instance;
    return instance;
}

void AssetManager::LoadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        throw std::runtime_error("Failed to load texture: " + filename);
    }
    textures[name] = std::move(texture);
}

const sf::Texture& AssetManager::GetTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    throw std::runtime_error("Texture not found: " + name);
}
