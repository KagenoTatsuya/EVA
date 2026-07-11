#pragma once
#include <SFML/System.hpp>
#include <cmath>

struct EquationDroite {
    float pente;
    float ordonneeOrigine;
    char type; // 'x' pour verticale, 'y' pour horizontale, sinon normale

    EquationDroite(float p, float o, char t = 'n')
        : pente(p), ordonneeOrigine(o), type(t) {
    }

    sf::Vector2f trouverIntersectionAvecBord(sf::Vector2f arrivee, float minX, float minY, float width, float height) {
        sf::Vector2f point = arrivee;
        if (type == 'x') {
            point.x = ordonneeOrigine;
            point.y = (arrivee.y < minY) ? minY : height;
        }
        else if (type == 'y') {
            point.y = ordonneeOrigine;
            point.x = (arrivee.x < minX) ? minX : width;
        }
        else {
            if (arrivee.x < minX) {
                point.x = minX;
                point.y = pente * point.x + ordonneeOrigine;
            }
            else if (arrivee.x > width) {
                point.x = width;
                point.y = pente * point.x + ordonneeOrigine;
            }
            else if (arrivee.y < minY) {
                point.y = minY;
                point.x = (point.y - ordonneeOrigine) / pente;
            }
            else if (arrivee.y > height) {
                point.y = height;
                point.x = (point.y - ordonneeOrigine) / pente;
            }
        }
        return point;
    }
};

class Deplacement {
public:
    static sf::Vector2f getPointArrive(sf::Vector2f depart, sf::Vector2f arrivee, float vitesse,
        float minX, float minY, float width, float height);
private:
    static sf::Vector2f ajoutDeplacementToPosition(sf::Vector2f depart, sf::Vector2f arrivee,
        sf::Vector2f deplacement, float vitesse);
    static EquationDroite calculEquationDroite(sf::Vector2f point1, sf::Vector2f point2);
    static float orienteVitesseAvecDirection(float vitesse, float direction);
    static float positif(float number);
};