#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "Animator.h"
#include "Block.h"
#include <random>
#include <array>
#include <vector>

class ProjectilePool;
class ZoneManager;
class WallGrid;

class Battle {
public:
    sf::RectangleShape rect;
    float vx, vy;
    bool alive;
    float timeAlive;

    Battle(sf::RectangleShape r, float vx, float vy)
        : rect(r), vx(vx), vy(vy), alive(true), timeAlive(0.f) {
    }

    virtual void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }, std::vector<Block*>* blocks = nullptr, const WallGrid* grid = nullptr, sf::Vector2f playerPos = { 0.f, 0.f }, bool playerAlive = false);
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

enum class Patterne { Kamikaze };

class Soldat : public Battle {
private:
    Team team;
    Patterne patterne;
    SoldatRole m_role = SoldatRole::Unassigned;
    float m_health = 100.f;
    float m_attackDamage = 20.f;
    float m_attackInterval = 1.f;
    float m_attackCooldown = 0.f;
    float m_speed = 80.f;

    int m_aiTickOffset = rand() % 4; // décalage aléatoire à la construction
    sf::Vector2f m_moveTarget{ 0.f, 0.f }; // dernière destination décidée par l'IA, réutilisée entre deux ticks

    sf::Vector2f m_lastKnownEnemyPos{ 0.f, 0.f };
    bool m_hasLastKnownEnemyPos = false;
    float m_searchTimeRemaining = 0.f;
    static constexpr float kSearchDuration = 3.f; // secondes passées à chercher avant d'abandonner

    bool m_isPursuingPlayer = false;
    static constexpr float kPlayerPursuitTriggerRadius = 150.f; // rayon de déclenchement de la poursuite kamikaze

    // Diversité de destination dans une zone
    char m_assignedZone = '\0';   // symbole de la Zone visée, au lieu d'un pointeur
    char m_targetZoneSymbol = '\0';     // zone attribuée pour l'attaque
    sf::Vector2f m_personalCapturePoint{ 0.f, 0.f };
    bool m_hasCapturePoint = false;

    sf::Vector2f m_personalWaypoint{ 0.f, 0.f };
    int m_personalWaypointId = -1;
    bool m_hasWaypoint = false;

    // Retiré : les surcharges sans "grid" n'étaient jamais définies dans le .cpp
    // (elles auraient provoqué une erreur de link si un jour quelqu'un les appelle).
    bool WouldCollide(sf::Vector2f testPos, const std::vector<Block*>& blocks, const WallGrid* grid) const;
    sf::Vector2f ComputeSteering(sf::Vector2f target, const std::vector<Block*>& blocks, const WallGrid* grid) const;

    // --- Détection et déblocage anti-coincement, avec itinéraire de secours dirigé par équipe ---
    sf::Vector2f m_stuckCheckPos{ 0.f, 0.f };
    float m_stuckCheckTimer = 0.f;
    int m_stuckStrikes = 0;
    static constexpr float kStuckCheckInterval = 0.5f;   // fréquence de vérification
    static constexpr float kStuckMoveThreshold = 25.f;   // déplacement minimum attendu entre deux checks
    static constexpr int   kStuckStrikesToEscape = 2;    // nb de checks consécutifs immobiles avant déclenchement (~1s)

    enum class EscapePhase { None, MovingSide, MovingUp };
    EscapePhase m_escapePhase = EscapePhase::None;
    sf::Vector2f m_escapePhaseStartPos{ 0.f, 0.f };
    float m_escapeSafetyTimer = 0.f;
    static constexpr float kEscapeStepDist = 5.f;      // distance de sonde pour détecter le mur devant
    static constexpr float kEscapeMaxUpDistance = 150.f; // distance verticale max avant d'abandonner la manœuvre (sécurité)
    static constexpr float kEscapeMaxPhaseDuration = 10.f; // sécurité anti-boucle infinie par phase

    sf::Vector2f ComputeEscapeTarget(const std::vector<Block*>& blocks, const WallGrid* grid) const;

public:
    float timeAlive;
    float phaseX;
    float phaseY;
    int cycleX;
    sf::Vector2f center;
    bool hasSprite = false;
    bool HasValidWaypoint() const;
    float GetHealth() const { return m_health; };
    int GetAITickOffset() const { return m_aiTickOffset; }
    sf::Vector2f GetMoveTarget() const { return m_moveTarget; }
    void SetMoveTarget(sf::Vector2f target) { m_moveTarget = target; }

    void SetLastKnownEnemyPos(sf::Vector2f pos) {
        m_lastKnownEnemyPos = pos;
        m_hasLastKnownEnemyPos = true;
        m_searchTimeRemaining = kSearchDuration;
    }
    bool HasLastKnownEnemyPos() const { return m_hasLastKnownEnemyPos; }
    sf::Vector2f GetLastKnownEnemyPos() const { return m_lastKnownEnemyPos; }
    void ClearLastKnownEnemyPos() { m_hasLastKnownEnemyPos = false; }
    void TickSearchTimer(float dt) {
        if (!m_hasLastKnownEnemyPos) return;
        m_searchTimeRemaining -= dt;
        if (m_searchTimeRemaining <= 0.f) ClearLastKnownEnemyPos();
    }

    bool HasLineOfSight(sf::Vector2f targetPos, const std::vector<Block*>& blocks, const WallGrid* grid = nullptr) const;
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
    bool TryAttack(Soldat* target, float dt, float attackRange, std::vector<SoldatProjectile*>& projectiles, const std::vector<Block*>& blocks, ProjectilePool& pool, const WallGrid* grid = nullptr);
    void TakeDamage(float dmg);

    sf::Vector2f GetOrAssignZonePoint(char zoneSymbol, const sf::FloatRect& zoneBounds);
    sf::Vector2f GetOrAssignWaypoint(char zoneSymbol, class ZoneManager& zoneManager, sf::Vector2f myPos);

    Soldat()
        : Battle(sf::RectangleShape(sf::Vector2f(60.f, 40.f)), 0.f, -1.f)
        , patterne(Patterne::Kamikaze) // valeur par défaut explicite, patterne était non initialisé sinon
    {
        rect.setPosition({ 0.f, 0.f });
    }

    ~Soldat();

    bool IsPursuingPlayer() const { return m_isPursuingPlayer; }

    Soldat(float x, float y, Team team, Patterne p);
    void ResolveCollisionsSold(std::vector<Block*>& blocks);
    void Update(float dt, sf::Vector2f cible = { 0.f, 0.f }, std::vector<Block*>* blocks = nullptr, const WallGrid* grid = nullptr, sf::Vector2f playerPos = { 0.f, 0.f }, bool playerAlive = false) override;
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
    const int m_maxTotal = 8;

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