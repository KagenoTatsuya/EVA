#pragma once
#include "BehaviorTree.h"
#include "ZoneManager.h"
#include "Battle.h"
#include <vector>

struct SoldatBlackboard {
    Soldat* self = nullptr;
    std::vector<Soldat*>* allSoldats = nullptr;
    ZoneManager* zoneManager = nullptr;
    float dt = 0.f;

    float attackRange = 150.f;
    float chaseRange = 350.f;
    float retreatHealthThreshold = 25.f;

    Soldat* targetEnemy = nullptr;
    Zone* targetZone = nullptr;
    sf::Vector2f moveTarget{ 0.f, 0.f };
    std::vector<SoldatProjectile*>* projectiles = nullptr;
    const std::vector<Block*>* blocks = nullptr;
};

Soldat* FindNearestEnemy(SoldatBlackboard& bb, float maxRange);
void AssignSoldatRoles(std::vector<Soldat*>& soldats, ZoneManager& zoneManager, Team team);
BTNodePtr BuildSoldatBehaviorTree();