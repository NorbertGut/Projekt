#ifndef SYMULACJA_H
#define SYMULACJA_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "ObiektySymulacji.h"
#include "Rury.h"
#include "Nakladki.h"
#include "Etykiety.h"

class Sterowanie;
class Animacje;

enum class TrybPracy {
    AUTOMATYCZNY,
    RECZNY
};

class Symulacja {
    friend class Sterowanie;  
    friend class Animacje;    
    friend class Etykiety;
    friend class ZapisPlikC;

private:
    sf::RenderWindow okno;
    sf::Font czcionka;
    Etykiety ety;
    
    TrybPracy aktualnyTryb;
    bool pokazWidokUproszczony;
    bool grzanieBoileraReczne;
    bool czyWyjsciePotwierdzone;

   
    Zbiornik zbior1, zbior2, zbior3, zbior4;

   
    Rury rury;

    
    Pompa pompa1, pompa2, pompa3;

    
public:
    const int liczbaKropli0, liczbaKropli1, liczbaKropli;
private:
    SciezkaKropli sciezka0, sciezka1, sciezka2, sciezka3;
    std::vector<sf::CircleShape> krople0, krople1, krople2, krople3;
    std::vector<float> dyst0, dyst1, dyst2, dyst3;

    
    float predkosc0, predkosc1, predkosc2, predkosc3;

   
    float temperaturaZadana3, temperaturaZadana4;

   
    float odplyw3, odplyw4;

    float histereza;

    bool pokazPomoc; 
    sf::Clock zegar;

   
    Nakladki nakladki;

   
    void obsluzZdarzenia();
    void aktualizuj(float dt);
    void renderuj();



    void rysujKontrolkeTrybu();
    void rysujEtykieteZapisu();
    void rysujEtykietyZbiornikow();

public:
    Symulacja();
    void zapiszStanDoPliku(const std::string& nazwa);
    void wczytajStanZPliku(const std::string& nazwa);
    void inicjalizuj();
    void uruchom();
};

#endif 
