#include "Deplacement.h"

sf::Vector2f Deplacement::getPointArrive(sf::Vector2f depart, sf::Vector2f arrivee, float vitesse,
    float minX, float minY, float width, float height) {
    if (arrivee.x < minX || arrivee.x > width || arrivee.y < minY || arrivee.y > height) {
        EquationDroite equationDroite = calculEquationDroite(depart, arrivee);
        arrivee = equationDroite.trouverIntersectionAvecBord(arrivee, minX, minY, width, height);
    }

    float xd = arrivee.x - depart.x;
    float yd = arrivee.y - depart.y;
    float hypotenuse = std::sqrt(xd * xd + yd * yd);

    if (hypotenuse == 0.f) {
        return depart;
    }

    float vitesseX = positif(xd) / hypotenuse * vitesse;
    float vitesseY = positif(yd) / hypotenuse * vitesse;

    vitesseX = orienteVitesseAvecDirection(vitesseX, xd);
    vitesseY = orienteVitesseAvecDirection(vitesseY, yd);

    return ajoutDeplacementToPosition(depart, arrivee, sf::Vector2f{ vitesseX, vitesseY }, vitesse);
}

sf::Vector2f Deplacement::ajoutDeplacementToPosition(sf::Vector2f depart, sf::Vector2f arrivee,
    sf::Vector2f deplacement, float vitesse) {
    float diffX = arrivee.x - depart.x;
    float diffY = arrivee.y - depart.y;

    if (std::sqrt(diffX * diffX + diffY * diffY) <= vitesse) {
        return arrivee;
    }

    return sf::Vector2f{ depart.x + deplacement.x, depart.y + deplacement.y };
}

EquationDroite Deplacement::calculEquationDroite(sf::Vector2f p1, sf::Vector2f p2) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    if (dx == 0.f) return EquationDroite(0.f, p1.x, 'x');
    if (dy == 0.f) return EquationDroite(0.f, p1.y, 'y');

    float pente = dy / dx;
    float ordonneeOrigine = p1.y - pente * p1.x;
    return EquationDroite(pente, ordonneeOrigine);
}

float Deplacement::orienteVitesseAvecDirection(float vitesse, float direction) {
    return (direction > 0) ? vitesse : -vitesse;
}

float Deplacement::positif(float number) {
    return (number < 0) ? -number : number;
}