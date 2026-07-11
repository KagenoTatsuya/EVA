#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Battle.h" // Team

enum class ZoneOwner {
    Neutral,
    Orange,
    Bleu
};

struct Zone {
    char symbol;                  // lettre d'origine dans le fichier de niveau ('K', 'J'...)
    std::string displayName;      // nom affiché ("Zone B", "Zone C"...)
    sf::FloatRect bounds;
    ZoneOwner owner = ZoneOwner::Neutral;
    float captureProgress = 0.f;  // -100 (Orange) .. 0 (neutre) .. +100 (Bleu)

    static constexpr float kCaptureMax = 100.f;
    static constexpr float kCaptureSpeed = 15.f; // points/sec par soldat en surplus

    sf::Vector2f GetCenter() const {
        return { bounds.position.x + bounds.size.x / 2.f,
                 bounds.position.y + bounds.size.y / 2.f };
    }

    // Renvoie le % de contrôle de l'équipe donnée (0 à 100)
    float GetCapturePercent(Team team) const {
        // captureProgress va de -100 (100% Orange) à +100 (100% Bleu)
        float bleuPercent = (captureProgress + kCaptureMax) / (2.f * kCaptureMax) * 100.f;
        return (team == Team::Bleu) ? bleuPercent : (100.f - bleuPercent);
    }
};