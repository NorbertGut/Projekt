#include "Animacje.h"
#include "Symulacja.h"
#include <SFML/Graphics.hpp>

void Animacje::aktualizujKrople(Symulacja& sym, float dt)
{
    

    for (int i = 0; i < sym.liczbaKropli0; ++i) {
        sym.dyst0[i] += sym.predkosc0 * dt;
        if (sym.dyst0[i] > sym.sciezka0.calkowitaDlugosc)
            sym.dyst0[i] -= sym.sciezka0.calkowitaDlugosc;
    }
    for (int i = 0; i < sym.liczbaKropli1; ++i) {
        sym.dyst1[i] += sym.predkosc1 * dt;
        if (sym.dyst1[i] > sym.sciezka1.calkowitaDlugosc)
            sym.dyst1[i] -= sym.sciezka1.calkowitaDlugosc;
    }
    for (int i = 0; i < sym.liczbaKropli; ++i) {
        sym.dyst2[i] += sym.predkosc2 * dt;
        if (sym.dyst2[i] > sym.sciezka2.calkowitaDlugosc)
            sym.dyst2[i] -= sym.sciezka2.calkowitaDlugosc;

        sym.dyst3[i] += sym.predkosc3 * dt;
        if (sym.dyst3[i] > sym.sciezka3.calkowitaDlugosc)
            sym.dyst3[i] -= sym.sciezka3.calkowitaDlugosc;
    }
}

void Animacje::rysujKrople(Symulacja& sym)
{
    
    for (int i = 0; i < sym.liczbaKropli0; ++i) {
        sf::Vector2f p = sym.sciezka0.wyznaczPozycje(sym.dyst0[i]);
        sym.krople0[i].setPosition(p - sf::Vector2f(sym.krople0[i].getRadius(), sym.krople0[i].getRadius()));
        sym.okno.draw(sym.krople0[i]);
    }
    for (int i = 0; i < sym.liczbaKropli1; ++i) {
        sf::Vector2f p = sym.sciezka1.wyznaczPozycje(sym.dyst1[i]);
        sym.krople1[i].setPosition(p - sf::Vector2f(sym.krople1[i].getRadius(), sym.krople1[i].getRadius()));
        sym.okno.draw(sym.krople1[i]);
    }
    for (int i = 0; i < sym.liczbaKropli; ++i) {
        sf::Vector2f p2 = sym.sciezka2.wyznaczPozycje(sym.dyst2[i]);
        sym.krople2[i].setPosition(p2 - sf::Vector2f(sym.krople2[i].getRadius(), sym.krople2[i].getRadius()));
        sym.okno.draw(sym.krople2[i]);

        sf::Vector2f p3 = sym.sciezka3.wyznaczPozycje(sym.dyst3[i]);
        sym.krople3[i].setPosition(p3 - sf::Vector2f(sym.krople3[i].getRadius(), sym.krople3[i].getRadius()));
        sym.okno.draw(sym.krople3[i]);
    }
}

void Animacje::aktualizujPompy(Symulacja& sym, float dt)
{
   
    sym.pompa1.obroc(sym.predkosc1 * dt);
    sym.pompa2.obroc(sym.predkosc2 * dt);
    sym.pompa3.obroc(sym.predkosc3 * dt);
}

void Animacje::rysujPompy(Symulacja& sym)
{
    sym.pompa1.rysuj(sym.okno);
    sym.pompa2.rysuj(sym.okno);
    sym.pompa3.rysuj(sym.okno);
}
