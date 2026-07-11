#include "SoldatAI.h"

Soldat* FindNearestEnemy(SoldatBlackboard& bb, float maxRange) {
    Soldat* best = nullptr;
    float bestDist = maxRange * maxRange;
    sf::Vector2f myPos = bb.self->rect.getPosition();
    for (Soldat* other : *bb.allSoldats) {
        if (other == bb.self || !other->alive) continue;
        if (other->GetTeam() == bb.self->GetTeam()) continue;
        sf::Vector2f d = other->rect.getPosition() - myPos;
        float dist = d.x * d.x + d.y * d.y;
        if (dist <= bestDist) { bestDist = dist; best = other; }
    }
    return best;
}

void AssignSoldatRoles(std::vector<Soldat*>& soldats, ZoneManager& zoneManager, Team team) {
    ZoneOwner myOwner = (team == Team::Orange) ? ZoneOwner::Orange : ZoneOwner::Bleu;

    auto& zones = zoneManager.GetZones();
    bool ownsAny = false, ownsAll = true;
    std::vector<char> contestable; // zones pas encore à nous
    for (Zone& z : zones) {
        if (z.owner == myOwner) ownsAny = true;
        else { ownsAll = false; contestable.push_back(z.symbol); }
    }

    std::vector<Soldat*> teamSoldats;
    for (Soldat* s : soldats)
        if (s->alive && s->GetTeam() == team) teamSoldats.push_back(s);

    if (teamSoldats.empty()) return;

    if (!ownsAny) {
        for (Soldat* s : teamSoldats) s->SetRole(SoldatRole::Attack);
    }
    else if (ownsAll) {
        for (Soldat* s : teamSoldats) s->SetRole(SoldatRole::Defend);
        return; // rien à contester, pas besoin de cible d'attaque
    }
    else {
        size_t targetDefend = teamSoldats.size() / 2;
        size_t currentDefend = 0;
        for (Soldat* s : teamSoldats)
            if (s->GetRole() == SoldatRole::Defend) currentDefend++;

        for (Soldat* s : teamSoldats) {
            if (s->GetRole() != SoldatRole::Unassigned) continue;
            if (currentDefend < targetDefend) { s->SetRole(SoldatRole::Defend); currentDefend++; }
            else s->SetRole(SoldatRole::Attack);
        }

        if (currentDefend > targetDefend) {
            for (Soldat* s : teamSoldats) {
                if (currentDefend <= targetDefend) break;
                if (s->GetRole() == SoldatRole::Defend) { s->SetRole(SoldatRole::Attack); currentDefend--; }
            }
        }
        else if (currentDefend < targetDefend) {
            for (Soldat* s : teamSoldats) {
                if (currentDefend >= targetDefend) break;
                if (s->GetRole() == SoldatRole::Attack) { s->SetRole(SoldatRole::Defend); currentDefend++; }
            }
        }
    }

    // --- Répartition round-robin des attaquants sur les zones contestables ---
    if (contestable.empty()) return;
    int attackIndex = 0;
    for (Soldat* s : teamSoldats) {
        if (s->GetRole() != SoldatRole::Attack) continue;
        char current = s->GetTargetZoneSymbol();
        bool stillValid = std::find(contestable.begin(), contestable.end(), current) != contestable.end();
        if (!stillValid) {
            s->SetTargetZoneSymbol(contestable[attackIndex % contestable.size()]);
        }
        attackIndex++;
    }
}

BTNodePtr BuildSoldatBehaviorTree() {
    return MakeSelector(
        // --- Branche combat : priorité sur la capture de zone ---
        MakeSequence(
            MakeCondition([](SoldatBlackboard& bb) {
                bb.targetEnemy = FindNearestEnemy(bb, bb.chaseRange);
                return bb.targetEnemy != nullptr;
                }),
            MakeAction([](SoldatBlackboard& bb) {
                sf::Vector2f myPos = bb.self->rect.getPosition();
                sf::Vector2f enemyPos = bb.targetEnemy->rect.getPosition();
                sf::Vector2f d = enemyPos - myPos;
                float dist = std::sqrt(d.x * d.x + d.y * d.y);

                if (dist <= bb.attackRange) {
                    // à portée : on reste sur place et on tire
                    bb.moveTarget = myPos;
                    bb.self->TryAttack(bb.targetEnemy, bb.dt, bb.attackRange, *bb.projectiles);
                }
                else {
                    // pas encore à portée : on avance mais pas jusqu'au corps à corps
                    sf::Vector2f dir = d / dist;
                    bb.moveTarget = enemyPos - dir * bb.attackRange; // s'arrête à distance de tir
                }
                return BTStatus::Success;
                })
        ),
        // --- Branche capture de zone ---
        MakeSequence(
            MakeCondition([](SoldatBlackboard& bb) {
                if (bb.self->GetRole() == SoldatRole::Defend) {
                    bb.targetZone = bb.zoneManager->GetZoneOwnedByTeam(bb.self->GetTeam());
                }
                else {
                    char sym = bb.self->GetTargetZoneSymbol();
                    bb.targetZone = (sym != '\0') ? bb.zoneManager->GetZoneBySymbol(sym)
                        : bb.zoneManager->GetNearestContestableZone(
                            bb.self->rect.getPosition(), bb.self->GetTeam());
                }
                return bb.targetZone != nullptr;
                }),
            MakeAction([](SoldatBlackboard& bb) {
                char zoneId = bb.targetZone->symbol;
                bb.moveTarget = bb.self->GetOrAssignZonePoint(zoneId, bb.targetZone->bounds);
                return BTStatus::Success;
                })
        )
    );
}