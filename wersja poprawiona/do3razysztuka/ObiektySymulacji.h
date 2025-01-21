#ifndef OBIEKTYSYMULACJI_H
#define OBIEKTYSYMULACJI_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>


class SciezkaKropli {
public:
    std::vector<sf::Vector2f> punkty;
    std::vector<float> dlugosci;
    float calkowitaDlugosc;

    SciezkaKropli();
    SciezkaKropli(const std::vector<sf::Vector2f>& pts);

    sf::Vector2f wyznaczPozycje(float dystans) const;
};


class Zbiornik {
public:
    sf::RectangleShape ramka;
    sf::RectangleShape woda;
    float procentWody;
    float temperatura;

    Zbiornik(const sf::Vector2f& rozmiar, const sf::Vector2f& pozycja);

    void ustawProcentWody(float p);
    float pobierzMaksPojemnosc() const;
    float pobierzAktualnaObjetosc() const;
    void dodajWode(float litry, float tIn);
    float pobierzWode(float litry);

    void rysuj(sf::RenderWindow& okno);
};


class Pompa {
public:
    sf::CircleShape obudowa;
    sf::RectangleShape ramie1, ramie2;

    Pompa();
    Pompa(float srednica, sf::Vector2f pozycja);

    void obroc(float kat);
    void rysuj(sf::RenderWindow& okno);
};

#endif 
