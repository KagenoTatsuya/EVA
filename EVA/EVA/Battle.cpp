#include "Battle.h"
#include "Deplacement.h"
#include "ZoneManager.h"    
#include <cmath>
#include <numbers>

namespace {
    const std::array<std::string, 3> kSoldatSpritesheetsOrange = {
        "character-spritesheetTO.png",
        "character-spritesheetTO2.png",
        "character-spritesheetTO3.png"
    };

    const std::array<std::string, 3> kSoldatSpritesheetsBleu = {
        "character-spritesheetTB.png",
        "character-spritesheetTB2.png",
        "character-spritesheetTB3.png"
    };

    std::string GetRandomSpritesheet(Team team) {
        static std::mt19937 rng(std::random_device{}());

        if (team == Team::Orange) {
            std::uniform_int_distribution<size_t> dist(0, kSoldatSpritesheetsOrange.size() - 1);
            return kSoldatSpritesheetsOrange[dist(rng)];
        }
        else {
            std::uniform_int_distribution<size_t> dist(0, kSoldatSpritesheetsBleu.size() - 1);
            return kSoldatSpritesheetsBleu[dist(rng)];
        }
    }
}

Soldat::Soldat(float x, float y, Team team)
    : Battle(sf::RectangleShape(sf::Vector2f(32.f, 48.f)), 0.f, -1.f)
    , timeAlive(0.f)
    , phaseX(0.f)
    , phaseY(0.f)
    , cycleX(0)
    , center{ 720.f, y }
    ,team(team)
{
    rect.setPosition({ x, y });

    hasSprite = animator.LoadTexture(GetRandomSpritesheet(team));

    // Chaque frame fait 32x32
    const int W = 64;
    const int H = 64;

    // Animation BAS
    std::vector<sf::IntRect> upFrames = {
        sf::IntRect({0 * W, 8 * H}, {W, H}),
        sf::IntRect({1 * W, 8 * H}, {W, H}),
        sf::IntRect({2 * W, 8 * H}, {W, H}),
        sf::IntRect({3 * W, 8 * H}, {W, H})
    };

    animator.AddAnimation("up", upFrames, 0.12f);

    // Animation GAUCHE
    std::vector<sf::IntRect> leftFrames = {
        sf::IntRect({0 * W, 9 * H}, {W, H}),
        sf::IntRect({1 * W, 9 * H}, {W, H}),
        sf::IntRect({2 * W, 9 * H}, {W, H}),
        sf::IntRect({3 * W, 9 * H}, {W, H})
    };

    animator.AddAnimation("left", leftFrames, 0.12f);

    // Animation HAUT
    std::vector<sf::IntRect> downFrames = {
        sf::IntRect({0 * W, 10 * H}, {W, H}),
        sf::IntRect({1 * W, 10 * H}, {W, H}),
        sf::IntRect({2 * W, 10 * H}, {W, H}),
        sf::IntRect({3 * W, 10 * H}, {W, H})
    };

    animator.AddAnimation("down", downFrames, 0.12f);

    // Animation DROITE
    std::vector<sf::IntRect> rightFrames = {
        sf::IntRect({0 * W, 11 * H}, {W, H}),
        sf::IntRect({1 * W, 11 * H}, {W, H}),
        sf::IntRect({2 * W, 11 * H}, {W, H}),
        sf::IntRect({3 * W, 11 * H}, {W, H})
    };

    animator.AddAnimation("right", rightFrames, 0.12f);

    // ======================= Animation Idle ==============================

    // BAS
    std::vector<sf::IntRect> upIdle = {
        sf::IntRect({0 * W, 22 * H}, {W, H}),
        sf::IntRect({1 * W, 22 * H}, {W, H})
    };

    animator.AddAnimation("upIdle", upIdle, 0.12f);

    // GAUCHE
    std::vector<sf::IntRect> leftIdle = {
        sf::IntRect({0 * W, 23 * H}, {W, H}),
        sf::IntRect({1 * W, 23 * H}, {W, H})
    };

    animator.AddAnimation("leftIdle", leftIdle, 0.12f);

    // HAUT
    std::vector<sf::IntRect> downIdle = {
        sf::IntRect({0 * W, 24 * H}, {W, H}),
        sf::IntRect({1 * W, 24 * H}, {W, H})
    };

    animator.AddAnimation("downIdle", downIdle, 0.12f);

    // DROITE
    std::vector<sf::IntRect> rightIdle = {
        sf::IntRect({0 * W, 25 * H}, {W, H}),
        sf::IntRect({1 * W, 25 * H}, {W, H})
    };

    animator.AddAnimation("rightIdle", rightIdle, 0.12f);

    animator.Play("down"); // animation par défaut
}

void Battle::Update(float dt, sf::Vector2f cible, std::vector<Block*>* blocks) {
    // comportement par défaut, ou vide
}

void Soldat::Update(float dt, sf::Vector2f cible, std::vector<Block*>* blocks) {
    timeAlive += dt;

    const float minX = -175.f, maxX = 1920.f, minY = -175.f, maxY = 1920.f;

    sf::Vector2f pos = rect.getPosition();
    sf::Vector2f size = rect.getSize();
    sf::Vector2f oldPos = pos;

    sf::Vector2f steeredTarget = blocks ? ComputeSteering(cible, *blocks) : cible;

    pos = Deplacement::getPointArrive(pos, steeredTarget, m_speed * dt, minX, minY, maxX, maxY);

    if (timeAlive >= 3.f) {
        pos.x = std::max(minX, std::min(pos.x, maxX - size.x));
        pos.y = std::max(minY, std::min(pos.y, maxY - size.y));
    }

    sf::Vector2f delta = pos - oldPos;
    Direction newDirection = lastDirection;

    if (std::abs(delta.x) > std::abs(delta.y)) {
        newDirection = (delta.x > 0.f) ? Direction::RIGHT : Direction::LEFT;
    }
    else if (std::abs(delta.y) > 0.01f) {
        newDirection = (delta.y > 0.f) ? Direction::DOWN : Direction::UP;
    }

    if (newDirection != lastDirection) {
        lastDirection = newDirection;
        switch (lastDirection) {
        case Direction::UP:    animator.Play("up");    break;
        case Direction::DOWN:  animator.Play("down");  break;
        case Direction::LEFT:  animator.Play("left");  break;
        case Direction::RIGHT: animator.Play("right"); break;
        }
    }

    rect.setPosition(pos);
    animator.Update(dt);
    if (animator.sprite)
        animator.sprite->setPosition(rect.getPosition());
}

bool Soldat::HasLineOfSight(sf::Vector2f targetPos, const std::vector<Block*>& blocks) const {
    sf::Vector2f start = rect.getPosition() + rect.getSize() / 2.f;
    sf::Vector2f end = targetPos + sf::Vector2f(16.f, 24.f); // approx centre de la cible

    sf::Vector2f dir = end - start;
    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist < 1.f) return true;

    dir /= dist;
    const float step = 8.f; // pas d'échantillonnage le long du rayon
    for (float t = 0.f; t < dist; t += step) {
        sf::Vector2f p = start + dir * t;
        for (Block* b : blocks) {
            if (b->GetBlockType() != "MBlock") continue; // seuls les murs bloquent la vue
            if (b->rect.getGlobalBounds().contains(p)) {
                return false; // un mur bloque la vue
            }
        }
    }
    return true;
}

bool Soldat::TryAttack(Soldat* target, float dt, float attackRange, std::vector<SoldatProjectile*>& projectiles, const std::vector<Block*>& blocks) {
    m_attackCooldown -= dt;
    if (m_attackCooldown > 0.f) return false;
    if (!target || !target->alive) return false;

    sf::Vector2f d = target->rect.getPosition() - rect.getPosition();
    float distSq = d.x * d.x + d.y * d.y;
    if (distSq > attackRange * attackRange) return false;

    // Ne tire pas s'il y a un mur entre lui et la cible
    if (!HasLineOfSight(target->rect.getPosition(), blocks)) return false;

    float dist = std::sqrt(distSq);
    sf::Vector2f dir = d / dist;
    const float projSpeed = 500.f;

    SoldatProjectile* p = new SoldatProjectile();
    p->pos = rect.getPosition();
    p->velocity = dir * projSpeed;
    p->damage = m_attackDamage;
    p->team = team;
    projectiles.push_back(p);

    m_attackCooldown = m_attackInterval;
    return true;
}

void SoldatProjectile::Update(float dt) {
    pos += velocity * dt;
    m_shape.setPosition(pos);
    m_shape.setFillColor(team == Team::Orange ? sf::Color(255, 140, 0) : sf::Color(60, 120, 255));
}

void SoldatProjectile::Render(sf::RenderTarget* target) {
    target->draw(m_shape);
}

void Soldat::TakeDamage(float dmg) {
    m_health -= dmg;
    if (m_health <= 0.f) {
        alive = false; // membre hérité de Battle, déjà utilisé par cleanup existant
    }
}

void Soldat::Render(sf::RenderTarget* target) {
    if (animator.sprite) {
        animator.sprite->setPosition(rect.getPosition());
        target->draw(*animator.sprite);
    }
    else {
        rect.setFillColor(sf::Color::Red);
        target->draw(rect);
    }
}

Soldat::~Soldat() {
}

void Soldat::ResolveCollisionsSold(std::vector<Block*>& blocks) {
    sf::FloatRect bounds = rect.getGlobalBounds();
    for (Block* b : blocks) {
        if (b->GetBlockType() != "MBlock") continue; // ne collisionne qu'avec les vrais murs

        sf::FloatRect bb = b->rect.getGlobalBounds();
        if (auto overlap = bounds.findIntersection(bb)) {
            float overlapX = std::min(bounds.position.x + bounds.size.x, bb.position.x + bb.size.x)
                - std::max(bounds.position.x, bb.position.x);
            float overlapY = std::min(bounds.position.y + bounds.size.y, bb.position.y + bb.size.y)
                - std::max(bounds.position.y, bb.position.y);
            sf::Vector2f pos = rect.getPosition();
            if (overlapX < overlapY) {
                pos.x += (bounds.position.x < bb.position.x) ? -overlapX : overlapX;
            }
            else {
                pos.y += (bounds.position.y < bb.position.y) ? -overlapY : overlapY;
            }
            rect.setPosition(pos);
            bounds = rect.getGlobalBounds();
        }
    }
}

//===============================================SPAWN Soldat================================================
float SoldatSpawnerB::GetWaveInterval() const {
    return 1.f;
}

void SoldatSpawnerB::Update(float dt, std::vector<Soldat*>& soldat, float spawnX, float spawnY) {
    // Compte combien de soldats Bleu sont actuellement vivants sur la carte
    int aliveCount = 0;
    for (Soldat* s : soldat) {
        if (s->GetTeam() == Team::Bleu) aliveCount++;
    }

    if (aliveCount >= 6) return; // limite atteinte, on n'ajoute rien

    if (m_soldatsQueued > 0) {
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.f) {
            soldat.push_back(new Soldat(spawnX, spawnY, Team::Bleu));
            m_soldatsQueued--;
            m_spawnTimer = 1.f;
            aliveCount++;

            if (aliveCount >= 6) return;
        }
        return;
    }

    m_waveTimer -= dt;
    if (m_waveTimer <= 0.f) {
        // On ne queue que ce qu'il manque pour atteindre 10 sur la carte
        m_soldatsQueued = 6 - aliveCount;
        m_spawnTimer = 0.f;
        m_waveTimer = GetWaveInterval();
    }
}

float SoldatSpawnerO::GetWaveInterval() const {
    return 1.f;
}

sf::Vector2f Soldat::GetOrAssignZonePoint(char zoneSymbol, const sf::FloatRect& zoneBounds) {
    if (m_assignedZone != zoneSymbol || !m_hasCapturePoint) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> distX(0.f, zoneBounds.size.x);
        std::uniform_real_distribution<float> distY(0.f, zoneBounds.size.y);

        m_personalCapturePoint = {
            zoneBounds.position.x + distX(rng),
            zoneBounds.position.y + distY(rng)
        };
        m_assignedZone = zoneSymbol;
        m_hasCapturePoint = true;
    }
    return m_personalCapturePoint;
}

sf::Vector2f Soldat::GetOrAssignWaypoint(char zoneSymbol, ZoneManager& zoneManager, sf::Vector2f myPos) {
    constexpr float kWaypointReachRadius = 24.f;

    bool needNew = !m_hasWaypoint || m_assignedZone != zoneSymbol;

    if (!needNew) {
        sf::Vector2f d = m_personalWaypoint - myPos;
        if (d.x * d.x + d.y * d.y <= kWaypointReachRadius * kWaypointReachRadius) {
            needNew = true; // arrivé : direction la prochaine étape du graphe
        }
    }

    if (needNew) {
        int newId = zoneManager.GetRandomWaypointId(m_personalWaypointId, myPos);
        if (newId != -1) {
            sf::Vector2f pos;
            if (zoneManager.GetWaypointById(newId, pos)) {
                m_personalWaypoint = pos;
                m_personalWaypointId = newId;
                m_hasWaypoint = true;
            }
        }
        m_assignedZone = zoneSymbol;
    }
    return m_personalWaypoint;
}

void SoldatSpawnerO::Update(float dt, std::vector<Soldat*>& soldat, float spawnX, float spawnY) {
    int aliveCount = 0;
    for (Soldat* s : soldat) {
        if (s->GetTeam() == Team::Orange) aliveCount++;
    }

    if (aliveCount >= 8) return;

    if (m_soldatsQueued > 0) {
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.f) {
            soldat.push_back(new Soldat(spawnX, spawnY, Team::Orange));
            m_soldatsQueued--;
            m_spawnTimer = 1.f;
            aliveCount++;

            if (aliveCount >= 8) return;
        }
        return;
    }

    m_waveTimer -= dt;
    if (m_waveTimer <= 0.f) {
        m_soldatsQueued = 10 - aliveCount;
        m_spawnTimer = 0.f;
        m_waveTimer = GetWaveInterval();
    }
}

bool Soldat::HasValidWaypoint() const {
    return m_hasWaypoint;
}

bool Soldat::WouldCollide(sf::Vector2f testPos, const std::vector<Block*>& blocks) const {
    sf::FloatRect testBounds(testPos, rect.getSize());
    for (Block* b : blocks) {
        if (b->GetBlockType() != "MBlock") continue; // ne teste que les vrais murs
        if (testBounds.findIntersection(b->rect.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

sf::Vector2f Soldat::ComputeSteering(sf::Vector2f target, const std::vector<Block*>& blocks) const {
    sf::Vector2f pos = rect.getPosition();
    sf::Vector2f toTarget = target - pos;
    float dist = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    if (dist < 1.f) return target;

    sf::Vector2f dir = toTarget / dist;
    const float probeDist = 40.f; // distance de sondage devant le soldat

    // Rien devant : trajectoire directe
    if (!WouldCollide(pos + dir * probeDist, blocks)) {
        return target;
    }

    // Obstacle détecté : on balaie des angles croissants de part et d'autre
    // de la direction voulue, du plus proche au plus large, pour trouver
    // la trajectoire libre la plus proche (permet de raser/effleurer le mur
    // au lieu de rester bloqué dessus).
    const float degToRad = 3.14159265f / 180.f;
    for (float angleDeg = 10.f; angleDeg <= 170.f; angleDeg += 10.f) {
        float angleRad = angleDeg * degToRad;
        float cosA = std::cos(angleRad);
        float sinA = std::sin(angleRad);

        // essai côté "droit"
        sf::Vector2f dirRight(
            dir.x * cosA - dir.y * sinA,
            dir.x * sinA + dir.y * cosA
        );
        if (!WouldCollide(pos + dirRight * probeDist, blocks)) {
            return pos + dirRight * dist;
        }

        // essai côté "gauche" (symétrique)
        sf::Vector2f dirLeft(
            dir.x * cosA + dir.y * sinA,
            -dir.x * sinA + dir.y * cosA
        );
        if (!WouldCollide(pos + dirLeft * probeDist, blocks)) {
            return pos + dirLeft * dist;
        }
    }

    // Vraiment coincé de tous les côtés (coin/impasse, cas rare) :
    // on recule légèrement au lieu de rester figé contre le mur.
    return pos - dir * (probeDist * 0.5f);
}