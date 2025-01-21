#ifndef NAKLADKI_H
#define NAKLADKI_H

#include <SFML/Graphics.hpp>

class Nakladki {
private:
    const sf::Font& font;

public:
    Nakladki(const sf::Font& f);

    void rysujPomoc(sf::RenderWindow& window);

    void rysujWidokUproszczony(sf::RenderWindow& window,
        float boilerTemp,
        float tempZb3,
        float tempZb4,
        float temperaturaZad3,
        float temperaturaZad4
    );

    void rysujNakladkeWyjscia(sf::RenderWindow& window);
};

#endif 
