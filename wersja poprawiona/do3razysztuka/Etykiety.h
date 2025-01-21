#pragma once
#ifndef ETYKIETY_H
#define ETYKIETY_H

#include <SFML/Graphics.hpp>
#include <string>


class Symulacja;


class Etykiety {
private:
    const sf::Font& font;

public:
    Etykiety(const sf::Font& f);

    
    void rysujEtykietyZbiornikow(sf::RenderWindow& window, const Symulacja& sym);

   
    void rysujKontrolkeTrybu(sf::RenderWindow& window, const Symulacja& sym);

    
    void rysujEtykieteZapisu(sf::RenderWindow& window);
};

#endif 
