#include "ProjectilePool.h"
#include "Battle.h" // définition complète de SoldatProjectile nécessaire ici (new, ->alive, delete)

SoldatProjectile* ProjectilePool::Acquire() {
    for (SoldatProjectile* p : m_pool) {
        if (!p->alive) {
            p->alive = true;
            return p;
        }
    }
    SoldatProjectile* p = new SoldatProjectile();
    m_pool.push_back(p);
    return p;
}

ProjectilePool::~ProjectilePool() {
    for (SoldatProjectile* p : m_pool) delete p;
}