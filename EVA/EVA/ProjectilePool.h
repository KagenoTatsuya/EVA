#pragma once
#include <vector>

class SoldatProjectile; // forward declaration, évite d'inclure Battle.h ici

class ProjectilePool {
public:
    ProjectilePool() = default;
    ~ProjectilePool();

    // Non copiable : le pool possède les projectiles, une copie créerait un double-free potentiel
    ProjectilePool(const ProjectilePool&) = delete;
    ProjectilePool& operator=(const ProjectilePool&) = delete;

    // Réutilise un projectile mort s'il y en a un disponible, sinon en crée un nouveau (une seule fois).
    SoldatProjectile* Acquire();

private:
    std::vector<SoldatProjectile*> m_pool; // possède TOUS les projectiles créés, morts ou vivants
};