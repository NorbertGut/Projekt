#include "Symulacja.h"
#include "Sterowanie.h"
#include "Animacje.h"
#include "Etykiety.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "ZapisPlikC.h"

Symulacja::Symulacja()
    : okno(sf::VideoMode(1000, 600), "System Kontroli Ogrzewania"),
    czcionka(),
    aktualnyTryb(TrybPracy::AUTOMATYCZNY),
    pokazWidokUproszczony(false),
    grzanieBoileraReczne(true),
    czyWyjsciePotwierdzone(false),
    zbior1({ 240.f,360.f }, { 50.f,120.f }),
    zbior2({ 120.f,180.f }, { 450.f,200.f }),
    zbior3({ 120.f,180.f }, { 820.f,50.f }),
    zbior4({ 120.f,180.f }, { 820.f,350.f }),
    pompa1(), pompa2(), pompa3(),
    liczbaKropli0(5), liczbaKropli1(10), liczbaKropli(20),
    sciezka0(), sciezka1(), sciezka2(), sciezka3(),
    krople0(liczbaKropli0), krople1(liczbaKropli1), krople2(liczbaKropli), krople3(liczbaKropli),
    dyst0(liczbaKropli0), dyst1(liczbaKropli1), dyst2(liczbaKropli), dyst3(liczbaKropli),
    predkosc0(0.f), predkosc1(0.f), predkosc2(0.f), predkosc3(0.f),
    temperaturaZadana3(40.f), temperaturaZadana4(50.f),
    odplyw3(5.f), odplyw4(3.f),
    histereza(3.f),
    pokazPomoc(false),
    nakladki(czcionka),
    ety(czcionka)
{
    okno.setFramerateLimit(60);
    if (!czcionka.loadFromFile("arial.ttf")) {
       
    }

    inicjalizuj();
}
void Symulacja::zapiszStanDoPliku(const std::string& nazwa)
{
    ZapisPlikC::zapiszStan(*this, nazwa);
}

void Symulacja::wczytajStanZPliku(const std::string& nazwa)
{
    ZapisPlikC::wczytajStan(*this, nazwa);
}

void Symulacja::inicjalizuj()
{
    zbior1.ustawProcentWody(100.f);
    zbior1.temperatura = 15.f;

    zbior2.ustawProcentWody(50.f);
    zbior2.temperatura = 15.f;

    zbior3.ustawProcentWody(30.f);
    zbior3.temperatura = 15.f;

    zbior4.ustawProcentWody(30.f);
    zbior4.temperatura = 15.f;

   
    rury.inicjalizuj(zbior1, zbior2, zbior3, zbior4);

    // Pozycje pomp
    {
        
        float cx = rury.getRura1().getPosition().x + rury.getRura1().getSize().x / 2.f;
        float cy = rury.getRura1().getPosition().y + rury.getRura1().getSize().y / 2.f;
        pompa1 = Pompa(36.f, sf::Vector2f(cx, cy));
    }
    {
        
        float x = rury.getRura2_h1().getPosition().x + rury.getRura2_h1().getSize().x;
        float y = rury.getRura2_h1().getPosition().y + rury.getRura2_h1().getSize().y / 2.f;
        pompa2 = Pompa(36.f, sf::Vector2f(x, y));
    }
    {
       
        float x = rury.getRura3_h1().getPosition().x + rury.getRura3_h1().getSize().x;
        float y = rury.getRura3_h1().getPosition().y + rury.getRura3_h1().getSize().y / 2.f;
        pompa3 = Pompa(36.f, sf::Vector2f(x, y));
    }

    
    {
        
        float sx = rury.getRura0().getPosition().x + (rury.getRura0().getSize().x / 2.f);
        float sy1 = 0.f;
        float sy2 = rury.getRura0().getPosition().y + rury.getRura0().getSize().y;
        std::vector<sf::Vector2f> raw0{
            {sx, sy1},
            {sx, sy2}
        };
        sciezka0 = SciezkaKropli(raw0);
    }
    {
        //zb1->zb2
        float sx = rury.getRura1().getPosition().x;
        float sy = rury.getRura1().getPosition().y + rury.getRura1().getSize().y / 2.f;
        float ex = sx + rury.getRura1().getSize().x;
        float ey = sy;
        std::vector<sf::Vector2f> raw1{
            {sx, sy}, {ex, ey}
        };
        sciezka1 = SciezkaKropli(raw1);
    }
    {
        //zb2->zb3 
        float sx = zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x;
        float sy = zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.25f;
        float tx = zbior3.ramka.getPosition().x - 100.f;
        float midY = zbior3.ramka.getPosition().y + zbior3.ramka.getSize().y / 2.f;
        std::vector<sf::Vector2f> raw2{
            {sx, sy},
            {tx, sy},
            {tx, midY},
            {zbior3.ramka.getPosition().x, midY}
        };
        sciezka2 = SciezkaKropli(raw2);
    }
    {
        //zb2->zb4
        float sx = zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x;
        float sy = zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.75f;
        float tx = zbior4.ramka.getPosition().x - 100.f;
        float midY = zbior4.ramka.getPosition().y + zbior4.ramka.getSize().y / 2.f;
        std::vector<sf::Vector2f> raw3{
            {sx, sy},
            {tx, sy},
            {tx, midY},
            {zbior4.ramka.getPosition().x, midY}
        };
        sciezka3 = SciezkaKropli(raw3);
    }

    // Krople
    float grubosc = 15.f;
    float rad = (grubosc / 2.f) - 1.f;
    for (int i = 0;i < liczbaKropli0;++i) {
        krople0[i].setRadius(rad);
        krople0[i].setFillColor(sf::Color::Blue);
        float frac = float(i) / float(liczbaKropli0);
        dyst0[i] = frac * sciezka0.calkowitaDlugosc;
    }
    for (int i = 0;i < liczbaKropli1;++i) {
        krople1[i].setRadius(rad);
        krople1[i].setFillColor(sf::Color::Blue);
        float frac = float(i) / float(liczbaKropli1);
        dyst1[i] = frac * sciezka1.calkowitaDlugosc;
    }
    for (int i = 0;i < liczbaKropli;++i) {
        krople2[i].setRadius(rad);
        krople3[i].setRadius(rad);
        krople2[i].setFillColor(sf::Color::Blue);
        krople3[i].setFillColor(sf::Color::Blue);

        float frac = float(i) / float(liczbaKropli);
        dyst2[i] = frac * sciezka2.calkowitaDlugosc;
        dyst3[i] = frac * sciezka3.calkowitaDlugosc;
    }
}

void Symulacja::uruchom()
{
    while (okno.isOpen()) {
        obsluzZdarzenia();
        float dt = zegar.restart().asSeconds();
        if (!czyWyjsciePotwierdzone) {
            aktualizuj(dt);
        }
        renderuj();
    }
}

void Symulacja::obsluzZdarzenia()
{
    sf::Event e;
    while (okno.pollEvent(e))
    {
        if (e.type == sf::Event::Closed)
        {
            
            czyWyjsciePotwierdzone = true;
        }
        else if (e.type == sf::Event::KeyPressed)
        {
            if (!czyWyjsciePotwierdzone)
            {
                
                if (e.key.code == sf::Keyboard::Escape) {
                    czyWyjsciePotwierdzone = true;
                }
                else if (e.key.code == sf::Keyboard::F1) {
                    pokazPomoc = !pokazPomoc;
                }
                else if (e.key.code == sf::Keyboard::M) {
                    if (aktualnyTryb == TrybPracy::AUTOMATYCZNY)
                        aktualnyTryb = TrybPracy::RECZNY;
                    else
                        aktualnyTryb = TrybPracy::AUTOMATYCZNY;
                }
                else if (e.key.code == sf::Keyboard::N) {
                    pokazWidokUproszczony = !pokazWidokUproszczony;
                }
                if (e.key.code == sf::Keyboard::K) {
                    ZapisPlikC::zapiszStan(*this, "plik.txt");
                }
                else if (e.key.code == sf::Keyboard::L) {
                    ZapisPlikC::wczytajStan(*this, "plik.txt");
                }

                
                if (e.key.code == sf::Keyboard::X) {
                    temperaturaZadana3 += 1.f;
                }
                else if (e.key.code == sf::Keyboard::Z) {
                    temperaturaZadana3 -= 1.f;
                }

                
                else if (e.key.code == sf::Keyboard::C) {
                    temperaturaZadana4 += 1.f;
                }
                else if (e.key.code == sf::Keyboard::V) {
                    temperaturaZadana4 -= 1.f;
                }

                
                if (aktualnyTryb == TrybPracy::RECZNY)
                {
                    if (e.key.code == sf::Keyboard::Q) predkosc0 += 10.f;
                    else if (e.key.code == sf::Keyboard::A) predkosc0 = std::max(0.f, predkosc0 - 10.f);
                    
                }
            }
            else
            {
                
                if (e.key.code == sf::Keyboard::T) {
                    okno.close();
                }
                else if (e.key.code == sf::Keyboard::N) {
                    czyWyjsciePotwierdzone = false;
                }

            }

        }
    }
}

void Symulacja::aktualizuj(float dt)
{
   
    if (aktualnyTryb == TrybPracy::AUTOMATYCZNY) {
        Sterowanie::aktualizujAutomatycznie(*this, dt);
    }
    else {
        Sterowanie::aktualizujRecznie(*this, dt);
    }

    
    Animacje::aktualizujKrople(*this, dt);
    Animacje::aktualizujPompy(*this, dt);
}

void Symulacja::renderuj()



{
    
    okno.clear(sf::Color(50, 50, 50));

    
    if (pokazWidokUproszczony) {
       
        nakladki.rysujWidokUproszczony(
            okno,
            zbior2.temperatura,
            zbior3.temperatura,
            zbior4.temperatura,
            temperaturaZadana3,
            temperaturaZadana4
        );
        
        ety.rysujEtykieteZapisu(okno);

        okno.display();
        return;
    }

    
    if (aktualnyTryb == TrybPracy::RECZNY && grzanieBoileraReczne) {
        zbior2.ramka.setOutlineColor(sf::Color::Red);
    }
    else {
        zbior2.ramka.setOutlineColor(sf::Color::White);
    }

    
    zbior1.rysuj(okno);
    zbior2.rysuj(okno);
    zbior3.rysuj(okno);
    zbior4.rysuj(okno);

    
    rury.rysuj(okno);

    
    Animacje::rysujKrople(*this);
    Animacje::rysujPompy(*this);

   
   
    ety.rysujEtykietyZbiornikow(okno, *this);  
    ety.rysujKontrolkeTrybu(okno, *this);      
    ety.rysujEtykieteZapisu(okno);             

    
    if (pokazPomoc) {
        nakladki.rysujPomoc(okno);
    }
    
    if (czyWyjsciePotwierdzone) {
        nakladki.rysujNakladkeWyjscia(okno);
    }

    
    okno.display();
}


