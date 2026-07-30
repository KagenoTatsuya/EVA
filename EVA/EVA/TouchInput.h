#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Gère les contrôles tactiles : un joystick virtuel "flottant" (zone gauche
// de l'écran) pour le déplacement, et un bouton de tir fixe (zone droite).
// À appeler une fois par frame via Update(), après avoir rempli le vecteur
// d'événements SFML de la frame.
class TouchInput {
public:
    TouchInput() = default;

    // Traite les événements tactiles de la frame courante.
    void Update(const std::vector<sf::Event>& events, sf::Vector2u windowSize);

    // Vecteur de déplacement du joystick, composantes dans [-1, 1].
    sf::Vector2f GetMovementVector() const { return m_moveVector; }

    // Vrai tant qu'un doigt maintient le bouton de tir appuyé.
    bool IsShootHeld() const { return m_shootHeld; }

    // Dessine le joystick et le bouton à l'écran. À appeler en vue par
    // défaut (window.setView(window.getDefaultView())), comme le HUD.
    void Render(sf::RenderWindow& window) const;

    // Permet de couper entièrement l'affichage/la lecture tactile
    // (par exemple si tu détectes que le jeu tourne sans écran tactile).
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool IsEnabled() const { return m_enabled; }

private:
    void ComputeLayoutIfNeeded(sf::Vector2u windowSize);
    static bool IsInCircle(sf::Vector2f point, sf::Vector2f center, float radius);

    bool m_enabled = true;

    // Layout recalculé seulement si la taille de fenêtre change
    sf::Vector2u m_lastWindowSize{ 0, 0 };
    sf::Vector2f m_shootButtonCenter;
    float m_shootButtonRadius = 70.f;
    float m_joystickMaxRadius = 90.f;
    float m_joystickBaseRadius = 45.f; // rayon visuel du cercle de base
    float m_joystickThumbRadius = 25.f; // rayon visuel du curseur

    // Joystick : un seul doigt actif à la fois, base "flottante"
    bool m_joystickActive = false;
    unsigned int m_joystickFingerId = 0;
    sf::Vector2f m_joystickBaseCenter;
    sf::Vector2f m_joystickThumbPos;
    sf::Vector2f m_moveVector{ 0.f, 0.f };

    // Bouton de tir : un doigt dédié
    bool m_shootActive = false;
    unsigned int m_shootFingerId = 0;
    bool m_shootHeld = false;
};