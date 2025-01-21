#ifndef RURY_H
#define RURY_H

#include <SFML/Graphics.hpp>
#include "ObiektySymulacji.h"

class Rury {
private:
    sf::RectangleShape rura0, rura1;
    sf::RectangleShape rura2_h1, rura2_v, rura2_h2;
    sf::RectangleShape rura3_h1, rura3_v, rura3_h2;

public:
    void inicjalizuj(const Zbiornik& zb1,
        const Zbiornik& zb2,
        const Zbiornik& zb3,
        const Zbiornik& zb4);

    void rysuj(sf::RenderWindow& okno);

    
    const sf::RectangleShape& getRura0() const { return rura0; }
    const sf::RectangleShape& getRura1() const { return rura1; }

    const sf::RectangleShape& getRura2_h1() const { return rura2_h1; }
    const sf::RectangleShape& getRura2_v()   const { return rura2_v; }
    const sf::RectangleShape& getRura2_h2() const { return rura2_h2; }

    const sf::RectangleShape& getRura3_h1() const { return rura3_h1; }
    const sf::RectangleShape& getRura3_v()   const { return rura3_v; }
    const sf::RectangleShape& getRura3_h2() const { return rura3_h2; }
};

#endif 
