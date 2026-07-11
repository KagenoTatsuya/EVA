#pragma once
#include <SFML/Graphics.hpp>
#include "Animator.h"
#include "Block.h"
#include <random>
#include <array>
#include <vector>

class ZoneManager;

class Battle {
public:
    sf::RectangleShape rect;
    float vx, vy;
    bool alive;
    float timeAlive;

    Battle(sf::RectangleShape r, float vx, float vy)
        : rect(r), vx(vx), vy(vy), alive(true), timeAlive(0.f) {
    }

    virtual void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }, std::vector<Block*>* blocks = nullptr);
    virtual void Render(sf::RenderTarget* target) = 0; // méthode virtuelle pure
    virtual ~Battle() = default;
};

enum class Team {
    Orange,
    Bleu
};

enum class SoldatRole {
    Unassigned,
    Attack,
    Defend
};

class SoldatProjectile; 

class Soldat : public Battle {
private:
    Team team;
    SoldatRole m_role = SoldatRole::Unassigned;
    float m_health = 100.f;
    float m_attackDamage = 10.f;
    float m_attackInterval = 1.f;
    float m_attackCooldown = 0.f;
    float m_speed = 80.f;

    // Diversité de destination dans une zone
    char m_assignedZone = '\0';   // symbole de la Zone visée, au lieu d'un pointeur
    char m_targetZoneSymbol = '\0';     // zone attribuée pour l'attaque
    sf::Vector2f m_personalCapturePoint{ 0.f, 0.f };
    bool m_hasCapturePoint = false;

    sf::Vector2f m_personalWaypoint{ 0.f, 0.f };
    int m_personalWaypointId = -1;
    bool m_hasWaypoint = false;
    bool WouldCollide(sf::Vector2f testPos, const std::vector<Block*>& blocks) const;
    sf::Vector2f ComputeSteering(sf::Vector2f target, const std::vector<Block*>& blocks) const;

public:
    float timeAlive;
    float phaseX;
    float phaseY;
    int cycleX;
    sf::Vector2f center;
    bool hasSprite = false;
    bool HasValidWaypoint() const;

    char GetTargetZoneSymbol() const { return m_targetZoneSymbol; }
    void SetTargetZoneSymbol(char c) { m_targetZoneSymbol = c; }

    enum class Direction {
        DOWN,
        LEFT,
        RIGHT,
        UP
    } state;

    Direction lastDirection = Direction::DOWN;
    Animator animator;

    Team GetTeam() const { return team; };
    SoldatRole GetRole() const { return m_role; };
    void SetRole(SoldatRole role) { m_role = role; };
    float GetHealth() const { return m_health; };

    bool TryAttack(Soldat* target, float dt, float attackRange, std::vector<SoldatProjectile*>& projectiles);
    void TakeDamage(float dmg);

    sf::Vector2f GetOrAssignZonePoint(char zoneSymbol, const sf::FloatRect& zoneBounds);
    sf::Vector2f GetOrAssignWaypoint(char zoneSymbol, class ZoneManager& zoneManager, sf::Vector2f myPos);

    Soldat()
        : Battle(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), 0.f, -1.f)
    {
        rect.setPosition({ 0.f, 0.f }); 
    }

    ~Soldat();

    Soldat(float x, float y, Team team);
    void ResolveCollisionsSold(std::vector<Block*>& blocks);
    void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }, std::vector<Block*>* blocks = nullptr) override;
    void Render(sf::RenderTarget* target) override;
};

class SoldatSpawnerB {  
public:
    SoldatSpawnerB() = default;

    void Update(float dt, std::vector<Soldat*>& soldat, float spawnX, float spawnY);

private:
    int m_totalSpawned = 0;
    const int m_maxTotal = 10;

    float m_waveTimer = 0.f;     // temps entre deux vagues
    float m_spawnTimer = 0.f;    // temps entre deux ennemis dans une vague
    int   m_soldatsQueued = 0;   // ennemis restants à spawn dans la vague en cours

    float GetWaveInterval() const;
};  

class SoldatSpawnerO {
public:
    SoldatSpawnerO() = default;
        
    void Update(float dt, std::vector<Soldat*>& soldat, float spawnX, float spawnY);

private:
    int m_totalSpawned = 0;
    const int m_maxTotal = 10;

    float m_waveTimer = 0.f;     // temps entre deux vagues
    float m_spawnTimer = 0.f;    // temps entre deux ennemis dans une vague
    int   m_soldatsQueued = 0;   // ennemis restants à spawn dans la vague en cours

    float GetWaveInterval() const;
};

class SoldatProjectile {
public:
    sf::Vector2f pos;
    sf::Vector2f velocity;
    float damage;
    Team team; // pour ne pas toucher son propre camp
    Soldat* targetHint = nullptr; // optionnel, pour dégâts garantis à l'arrivée
    bool alive = true;

    void Update(float dt);
    void Render(sf::RenderTarget* target);

private:
    sf::RectangleShape m_shape{ sf::Vector2f(6.f, 6.f) };
};