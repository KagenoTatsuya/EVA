#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

class TextureCache {
public:
    static TextureCache& Instance() {
        static TextureCache instance;
        return instance;
    }

    // Renvoie un pointeur vers la texture, chargée depuis le disque
    // une seule fois par chemin ; nullptr si le chargement échoue.
    sf::Texture* Get(const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) return it->second.get();

        auto tex = std::make_unique<sf::Texture>();
        if (!tex->loadFromFile(path)) {
            std::cerr << "Erreur chargement spritesheet: " << path << std::endl;
            return nullptr;
        }

        sf::Texture* raw = tex.get();
        m_textures[path] = std::move(tex);
        return raw;
    }

private:
    TextureCache() = default;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
};