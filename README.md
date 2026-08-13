# EVA

**EVA** est un jeu vidéo développé en C++ avec la bibliothèque [SFML](https://www.sfml-dev.org/), proposant plusieurs modes de jeu distincts avec une intelligence artificielle avancée, un système de capture de zones et une architecture technique optimisée pour la performance.

## 🎮 Modes de jeu

- **TPS / Running** — Mode d'action en vue à la troisième personne avec système de tir.
- **Survival / Zombie** — Mode de survie face à des vagues d'ennemis.
- **Battle** — Mode compétitif par équipes (Bleu vs Orange) avec capture de zones, IA de soldats autonomes et système de darkness (effet de cécité temporaire).

## ✨ Fonctionnalités principales

- **Intelligence artificielle par arbre de comportement (Behavior Tree)**
  Système à cinq branches priorisées : retraite en cas de faible santé, combat avec mémoire de dernière position connue, phase d'investigation, capture de zone, et patrouille sur points de passage (waypoints).

- **Navigation par graphe de waypoints**
  Graphe non-orienté et symétrique permettant aux soldats de se déplacer intelligemment entre les points numérotés de la carte.

- **Ligne de vue (Line of Sight)**
  Détection par échantillonnage de rayons (pas de 8px) filtrant les types de blocs, utilisée pour la détection d'ennemis.

- **Système anti-blocage (EscapePhase)**
  Logique d'échappement dirigée par équipe pour éviter que les soldats IA restent coincés contre les murs.

- **Capture de zones et fin de partie**
  Timer de bataille avec affichage critique sous 30 secondes, calcul du vainqueur par moyenne des pourcentages de capture de zone.

- **Optimisations de performance**
  - `ProjectilePool` — pool d'objets pour éviter les allocations répétées de projectiles.
  - `TextureCache` — singleton de cache de textures pour éviter les rechargements.
  - `WallGrid` — grille spatiale pour accélérer les raycasts de détection de murs.

- **Support tactile (Windows)**
  Joystick virtuel flottant et bouton de tir fixe via les événements tactiles SFML 3.

## 🛠️ Technologies

- **Langage** : C++
- **Bibliothèque graphique** : [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)
- **IDE recommandé** : Visual Studio

## 📦 Installation

### Prérequis

- Visual Studio (2019 ou supérieur recommandé)
- SFML (version compatible avec votre configuration)

### Compilation

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/<votre-utilisateur>/EVA.git
   cd EVA
   ```
2. Ouvrez le projet dans Visual Studio.
3. Configurez les chemins d'inclusion et de liaison vers SFML.
4. Compilez en configuration **Release** pour de meilleures performances (la configuration Debug peut entraîner des ralentissements notables, notamment en mode Battle).
5. Assurez-vous que les DLL SFML nécessaires (version release) se trouvent bien à côté de l'exécutable `EVA.exe`.

## 📁 Structure du projet

```
EVA/
├── src/
│   ├── Game.cpp / .h          # Machine à états principale, gestion des modes
│   ├── Entity.h                # Hiérarchie d'entités de base
│   ├── Joueur.cpp / .h         # Logique du joueur
│   ├── Soldat.cpp / .h         # IA des soldats
│   ├── SoldatAI.cpp            # Arbre de comportement des soldats
│   ├── BehaviorTree.h          # Système générique d'arbre de comportement
│   ├── ZoneManager.cpp / .h    # Gestion de la capture de zones
│   ├── DarknessEffect.cpp / .h # Effet de cécité (mode Battle)
│   ├── TouchInput.cpp / .h     # Support tactile
│   └── ...
├── assets/                     # Textures, sprites, niveaux
└── README.md
```

## 🕹️ Contrôles

| Action        | Touche / Entrée         |
|---------------|--------------------------|
| Déplacement   | ZQSD                     |
| Tir           | Clic gauche (visée souris) / ESPACE |
| Interaction   | E                        |

## 🧩 État du projet

Ce projet est en fini. Certaines fonctionnalités (comme l'intégration complète du support tactile) était en cours d'implémentation, malheureusement elles ne vairont pas le jour.

## 📄 Licence

BROUILLARD Théo

## 👤 Auteur

Développé par KagenoTatsuya.
