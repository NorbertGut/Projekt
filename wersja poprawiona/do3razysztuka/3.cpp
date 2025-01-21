#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

enum class TrybPracy {
    AUTOMATYCZNY,
    RECZNY
};

class Nakladki {
private:
    const sf::Font& font; 

public:
    
    Nakladki(const sf::Font& f) : font(f) {}

    // Nakładka pomocy (wcześniej rysujNakladkePomocy)
    void rysujPomoc(sf::RenderWindow& window) {
        sf::RectangleShape bg({ 600.f,300.f });
        bg.setFillColor(sf::Color(0, 0, 0, 150));
        bg.setPosition(200.f, 150.f);
        window.draw(bg);

        sf::Text txt(
            "POMOC:\n"
            " - [ESC] Wyjscie (z potwierdzeniem)\n"
            " - [F1] Pokaz/ukryj pomoc\n"
            " - [M] Przelaczanie trybu (AUTOMATYCZNY/RECZNY)\n"
            " - [N] Przelaczanie widoku (pelen/upro)\n"
            " - [K] Zapis stanu do pliku\n"
            " - [L] Odczyt stanu z pliku\n"
            " - W RECZNYM:\n"
            "   Q/A, W/S, E/D, R/F -> regulacja przeplywow\n"
            "   H -> wl/wy grzanie boilera\n"
            " - W AUTOMATYCZNYM:\n"
            "   X/Z -> Temp zadana CO +/-\n"
            "   C/V -> Temp zadana CWU +/-\n",
            font,
            18
        );
        txt.setFillColor(sf::Color::White);
        txt.setPosition(210.f, 160.f);
        window.draw(txt);
    }

   
    void rysujWidokUproszczony(
        sf::RenderWindow& window,
        float boilerTemp,
        float tempZb3,
        float tempZb4,
        float temperaturaZadana3,
        float temperaturaZadana4
    ) {
        sf::RectangleShape bg(sf::Vector2f(500.f, 180.f));
        bg.setFillColor(sf::Color(0, 0, 0, 200));
        float bx = (window.getSize().x - 500.f) * 0.5f;
        float by = (window.getSize().y - 180.f) * 0.5f;
        bg.setPosition(bx, by);
        window.draw(bg);

        sf::Text txt;
        txt.setFont(font);
        txt.setCharacterSize(18);
        txt.setFillColor(sf::Color::Yellow);

        std::ostringstream oss;
        oss << "UPROSZCZONY WIDOK\n\n"
            << "Aktualna temperatura boilera: " << (int)boilerTemp << "C,  docelowo: 90C\n"
            << "Aktualna temperatura CO: " << (int)tempZb3 << "C   Zadana: " << (int)temperaturaZadana3 << "C\n"
            << "Aktualna temperatura CWU: " << (int)tempZb4 << "C  Zadana: " << (int)temperaturaZadana4 << "C\n";

        txt.setString(oss.str());
        txt.setPosition(bx + 15.f, by + 15.f);
        window.draw(txt);
    }

    void rysujNakladkeWyjscia(sf::RenderWindow& window) {
        sf::RectangleShape bg({ 400.f,200.f });
        bg.setFillColor(sf::Color(0, 0, 0, 180));
        bg.setPosition(300.f, 200.f);
        window.draw(bg);

        sf::Text txt("Czy na pewno wyjsc?\n[T]ak / [N]ie", font, 24);
        txt.setFillColor(sf::Color::White);
        txt.setPosition(320.f, 230.f);
        window.draw(txt);
    }
};

class SciezkaKropli {
public:
    std::vector<sf::Vector2f> punkty;
    std::vector<float> dlugosci;
    float calkowitaDlugosc;

    SciezkaKropli() : calkowitaDlugosc(0.f) {}

    SciezkaKropli(const std::vector<sf::Vector2f>& pts)
        : punkty(pts), calkowitaDlugosc(0.f)
    {
        if (punkty.size() < 2) return;
        dlugosci.resize(punkty.size() - 1);
        for (size_t i = 0; i < punkty.size() - 1; ++i) {
            float dx = punkty[i + 1].x - punkty[i].x;
            float dy = punkty[i + 1].y - punkty[i].y;
            dlugosci[i] = std::sqrt(dx * dx + dy * dy);
            calkowitaDlugosc += dlugosci[i];
        }
    }

    sf::Vector2f wyznaczPozycje(float dystans) const {
        if (punkty.size() < 2) return { 0.f,0.f };
        float d = dystans;
        for (size_t i = 0; i < dlugosci.size(); ++i) {
            if (d <= dlugosci[i]) {
                float ratio = d / dlugosci[i];
                float x = punkty[i].x + ratio * (punkty[i + 1].x - punkty[i].x);
                float y = punkty[i].y + ratio * (punkty[i + 1].y - punkty[i].y);
                return { x,y };
            }
            d -= dlugosci[i];
        }
        return punkty.back();
    }
};

class Zbiornik {
public:
    sf::RectangleShape ramka;
    sf::RectangleShape woda;
    float procentWody;
    float temperatura;

    Zbiornik(const sf::Vector2f& rozmiar, const sf::Vector2f& pozycja)
        : procentWody(0.f), temperatura(15.f)
    {
        ramka.setSize(rozmiar);
        ramka.setPosition(pozycja);
        ramka.setFillColor(sf::Color::Transparent);
        ramka.setOutlineColor(sf::Color::White);
        ramka.setOutlineThickness(3.f);

        float szer = rozmiar.x - 6.f;
        float wys = rozmiar.y - 6.f;
        woda.setSize({ szer, 0.f });
        woda.setFillColor(sf::Color::Blue);
        woda.setPosition(pozycja.x + 3.f, pozycja.y + rozmiar.y - 3.f);
    }

    void ustawProcentWody(float p) {
        if (p < 0.f) p = 0.f;
        if (p > 100.f) p = 100.f;
        procentWody = p;

        float szer = ramka.getSize().x - 6.f;
        float wys = ramka.getSize().y - 6.f;
        float poziom = (p / 100.f) * wys;
        float top = ramka.getPosition().y + ramka.getSize().y - 3.f - poziom;
        woda.setSize({ szer, poziom });
        woda.setPosition(ramka.getPosition().x + 3.f, top);
    }

    float pobierzMaksPojemnosc() const {
        float szer = ramka.getSize().x - 6.f;
        float wys = ramka.getSize().y - 6.f;
        return szer * wys;
    }

    float pobierzAktualnaObjetosc() const {
        return (procentWody / 100.f) * pobierzMaksPojemnosc();
    }

    void dodajWode(float litry, float tIn) {
        if (litry <= 0.f) return;
        float maxV = pobierzMaksPojemnosc();
        float curV = pobierzAktualnaObjetosc();
        float wolne = maxV - curV;
        float faktyczne = std::min(wolne, litry);
        if (faktyczne > 0.f) {
            float nowaTemp = (temperatura * curV + tIn * faktyczne) / (curV + faktyczne);
            temperatura = nowaTemp;
            float nowaV = curV + faktyczne;
            float procent = (nowaV / maxV) * 100.f;
            ustawProcentWody(procent);
        }
    }

    float pobierzWode(float litry) {
        float curV = pobierzAktualnaObjetosc();
        float faktyczne = std::min(curV, litry);
        float pozostale = curV - faktyczne;
        float procent = (pozostale / pobierzMaksPojemnosc()) * 100.f;
        ustawProcentWody(procent);
        return faktyczne;
    }

    void rysuj(sf::RenderWindow& okno) {
        okno.draw(ramka);
        okno.draw(woda);
    }
};

// ---------------------------------------------------------
// Klasa Pompa (bez zmian)
// ---------------------------------------------------------
class Pompa {
public:
    sf::CircleShape obudowa;
    sf::RectangleShape ramie1, ramie2;

    Pompa() {}

    Pompa(float srednica, sf::Vector2f pozycja) {
        obudowa.setRadius(srednica / 2.f);
        obudowa.setOrigin(srednica / 2.f, srednica / 2.f);
        obudowa.setPosition(pozycja);
        obudowa.setFillColor(sf::Color(128, 128, 128));

        float gruboscKrzyza = srednica * 0.2f;
        float dlugoscKrzyza = srednica * 0.8f;

        ramie1.setSize({ gruboscKrzyza, dlugoscKrzyza });
        ramie1.setOrigin(gruboscKrzyza / 2.f, dlugoscKrzyza / 2.f);
        ramie1.setPosition(pozycja);
        ramie1.setFillColor(sf::Color::Black);

        ramie2.setSize({ dlugoscKrzyza, gruboscKrzyza });
        ramie2.setOrigin(dlugoscKrzyza / 2.f, gruboscKrzyza / 2.f);
        ramie2.setPosition(pozycja);
        ramie2.setFillColor(sf::Color::Black);
    }

    void obroc(float kat) {
        ramie1.rotate(kat);
        ramie2.rotate(kat);
    }

    void rysuj(sf::RenderWindow& okno) {
        okno.draw(obudowa);
        okno.draw(ramie1);
        okno.draw(ramie2);
    }
};

// ---------------------------------------------------------
// Klasa Symulacja - główna logika programu
// ---------------------------------------------------------
class Symulacja {
private:
    sf::RenderWindow okno;
    sf::Font czcionka;

    // Nowy obiekt Nakladki
    Nakladki nakladki;

    // Tryb i inne flagi
    TrybPracy aktualnyTryb;
    bool pokazWidokUproszczony;
    bool zaladujStan;
    bool zapiszStanPrzedZamknieciem;
    bool grzanieBoileraReczne;

    // Pomocnicze
    bool pokazPomoc;
    bool czyWyjsciePotwierdzone;

    sf::Clock zegar;

    // Zbiorniki
    Zbiornik zbior1, zbior2, zbior3, zbior4;

    // Rury (prostokąty)
    sf::RectangleShape rura0, rura1;
    sf::RectangleShape rura2_h1, rura2_v, rura2_h2;
    sf::RectangleShape rura3_h1, rura3_v, rura3_h2;

    // Krople i ścieżki
    SciezkaKropli sciezka0, sciezka1, sciezka2, sciezka3;
    const int liczbaKropli0 = 5, liczbaKropli1 = 10, liczbaKropli = 20;
    std::vector<sf::CircleShape> krople0, krople1, krople2, krople3;
    std::vector<float> dyst0, dyst1, dyst2, dyst3;

    // Pompy
    Pompa pompa1, pompa2, pompa3;

    // Prędkości
    float predkosc0, predkosc1, predkosc2, predkosc3;

    // Temperatury zadane
    float temperaturaZadana3, temperaturaZadana4;

    // Odpływy
    float odplyw3, odplyw4;

    // Histereza
    float histereza;

    // Metody prywatne:
    void inicjalizuj();
    void obsluzZdarzenia();
    void aktualizuj(float dt);
    void renderuj();

    void aktualizujGrzanieAutomatyczne(float dt);

    // Zapisywanie/odczyt do pliku
    void zapiszStanDoPliku(const std::string& nazwa);
    void wczytajStanZPliku(const std::string& nazwa);

    // Rysowanie
    void rysujKontrolkeTrybu();
    void rysujUIReczny() {} // (ew. placeholder, jeśli potrzebne)
    void rysujEtykieteZapisu();

    // Etykiety zbiorników
    void rysujEtykietyZbiornikow();

public:
    Symulacja();
    void uruchom();
};

Symulacja::Symulacja()
    : okno(sf::VideoMode(1000, 600), "System Kontroli Ogrzewania")
    , nakladki(czcionka) // inicjalizacja obiektu Nakladki (wymaga czcionki)
    , zbior1({ 240.f,360.f }, { 50.f,120.f })
    , zbior2({ 120.f,180.f }, { 450.f,200.f })
    , zbior3({ 120.f,180.f }, { 820.f,50.f })
    , zbior4({ 120.f,180.f }, { 820.f,350.f })
    , predkosc0(0.f), predkosc1(0.f), predkosc2(0.f), predkosc3(0.f)
    , temperaturaZadana3(40.f), temperaturaZadana4(50.f)
    , odplyw3(5.f), odplyw4(3.f)
    , histereza(3.f)
    , pokazPomoc(false), czyWyjsciePotwierdzone(false)
{
    okno.setFramerateLimit(60);

    aktualnyTryb = TrybPracy::AUTOMATYCZNY;
    pokazWidokUproszczony = false;
    zaladujStan = false;
    zapiszStanPrzedZamknieciem = true;
    grzanieBoileraReczne = true;

    if (!czcionka.loadFromFile("arial.ttf")) {
        // Obsługa błędu wczytania czcionki
        // np. std::cerr << "Blad wczytania czcionki!\n";
    }

    inicjalizuj();
}

void Symulacja::inicjalizuj() {
    zbior1.ustawProcentWody(100.f);
    zbior1.temperatura = 15.f;

    zbior2.ustawProcentWody(50.f);
    zbior2.temperatura = 15.f;

    zbior3.ustawProcentWody(30.f);
    zbior3.temperatura = 15.f;

    zbior4.ustawProcentWody(30.f);
    zbior4.temperatura = 15.f;

    float gruboscRury = 15.f;
    float polowa = gruboscRury / 2.f;

    // Rura0 (dopływ)
    {
        float sx = zbior1.ramka.getPosition().x + zbior1.ramka.getSize().x / 2.f - polowa;
        float sy = 0.f;
        float len = zbior1.ramka.getPosition().y - sy;
        rura0.setSize({ gruboscRury, len });
        rura0.setPosition(sx, sy);
        rura0.setFillColor(sf::Color::Black);
    }
    // Rura1 (zb1 -> zb2)
    {
        float sx = zbior1.ramka.getPosition().x + zbior1.ramka.getSize().x;
        float sy = zbior1.ramka.getPosition().y + zbior1.ramka.getSize().y / 2.f - polowa;
        float ex = zbior2.ramka.getPosition().x;
        float dl1 = ex - sx;
        rura1.setSize({ dl1, gruboscRury });
        rura1.setPosition(sx, sy);
        rura1.setFillColor(sf::Color::Black);
    }
    // Rura2 (zb2 -> zb3)
    {
        float sx = zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x;
        float sy = zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.25f - polowa;
        float tx = zbior3.ramka.getPosition().x - 100.f;
        float l1 = tx - sx;
        rura2_h1.setSize({ l1, gruboscRury });
        rura2_h1.setPosition(sx, sy);
        rura2_h1.setFillColor(sf::Color::Black);

        float ey = zbior3.ramka.getPosition().y + zbior3.ramka.getSize().y / 2.f - polowa;
        float l2 = ey - sy;
        rura2_v.setSize({ gruboscRury, std::abs(l2) });
        rura2_v.setPosition(tx, std::min(sy, ey));
        rura2_v.setFillColor(sf::Color::Black);

        float hx2 = zbior3.ramka.getPosition().x - tx;
        rura2_h2.setSize({ hx2, gruboscRury });
        rura2_h2.setPosition(tx, ey);
        rura2_h2.setFillColor(sf::Color::Black);
    }
    // Rura3 (zb2 -> zb4)
    {
        float sx = zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x;
        float sy = zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.75f - polowa;
        float tx = zbior4.ramka.getPosition().x - 100.f;
        float hx1 = tx - sx;
        rura3_h1.setSize({ hx1, gruboscRury });
        rura3_h1.setPosition(sx, sy);
        rura3_h1.setFillColor(sf::Color::Black);

        float ey = zbior4.ramka.getPosition().y + zbior4.ramka.getSize().y / 2.f - polowa;
        float vy = ey - sy;
        rura3_v.setSize({ gruboscRury, std::abs(vy) });
        rura3_v.setPosition(tx, std::min(sy, ey));
        rura3_v.setFillColor(sf::Color::Black);

        float hx2 = zbior4.ramka.getPosition().x - tx;
        rura3_h2.setSize({ hx2, gruboscRury });
        rura3_h2.setPosition(tx, ey);
        rura3_h2.setFillColor(sf::Color::Black);
    }

    // Ścieżki (sciezka0, sciezka1, sciezka2, sciezka3)
    {
        std::vector<sf::Vector2f> raw0{
            { zbior1.ramka.getPosition().x + zbior1.ramka.getSize().x / 2.f, 0.f },
            { zbior1.ramka.getPosition().x + zbior1.ramka.getSize().x / 2.f,
              zbior1.ramka.getPosition().y }
        };
        sciezka0 = SciezkaKropli(raw0);
    }
    {
        std::vector<sf::Vector2f> raw1{
            { zbior1.ramka.getPosition().x + zbior1.ramka.getSize().x,
              zbior1.ramka.getPosition().y + zbior1.ramka.getSize().y / 2.f},
            { zbior2.ramka.getPosition().x,
              zbior1.ramka.getPosition().y + zbior1.ramka.getSize().y / 2.f}
        };
        sciezka1 = SciezkaKropli(raw1);
    }
    {
        std::vector<sf::Vector2f> raw2{
            { zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x,
              zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.25f},
            { zbior3.ramka.getPosition().x - 100.f,
              zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.25f},
            { zbior3.ramka.getPosition().x - 100.f,
              zbior3.ramka.getPosition().y + zbior3.ramka.getSize().y / 2.f},
            { zbior3.ramka.getPosition().x,
              zbior3.ramka.getPosition().y + zbior3.ramka.getSize().y / 2.f}
        };
        sciezka2 = SciezkaKropli(raw2);
    }
    {
        std::vector<sf::Vector2f> raw3{
            { zbior2.ramka.getPosition().x + zbior2.ramka.getSize().x,
              zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.75f},
            { zbior4.ramka.getPosition().x - 100.f,
              zbior2.ramka.getPosition().y + zbior2.ramka.getSize().y * 0.75f},
            { zbior4.ramka.getPosition().x - 100.f,
              zbior4.ramka.getPosition().y + zbior4.ramka.getSize().y / 2.f},
            { zbior4.ramka.getPosition().x,
              zbior4.ramka.getPosition().y + zbior4.ramka.getSize().y / 2.f}
        };
        sciezka3 = SciezkaKropli(raw3);
    }

    // Krople
    krople0.resize(liczbaKropli0, sf::CircleShape((gruboscRury / 2.f) - 1.f));
    krople1.resize(liczbaKropli1, sf::CircleShape((gruboscRury / 2.f) - 1.f));
    krople2.resize(liczbaKropli, sf::CircleShape((gruboscRury / 2.f) - 1.f));
    krople3.resize(liczbaKropli, sf::CircleShape((gruboscRury / 2.f) - 1.f));
    dyst0.resize(liczbaKropli0);
    dyst1.resize(liczbaKropli1);
    dyst2.resize(liczbaKropli);
    dyst3.resize(liczbaKropli);

    for (int i = 0; i < liczbaKropli0; ++i) {
        float frac = float(i) / float(liczbaKropli0);
        dyst0[i] = frac * sciezka0.calkowitaDlugosc;
        krople0[i].setFillColor(sf::Color::Blue);
    }
    for (int i = 0; i < liczbaKropli1; ++i) {
        float frac = float(i) / float(liczbaKropli1);
        dyst1[i] = frac * sciezka1.calkowitaDlugosc;
        krople1[i].setFillColor(sf::Color::Blue);
    }
    for (int i = 0; i < liczbaKropli; ++i) {
        float frac = float(i) / float(liczbaKropli);
        dyst2[i] = frac * sciezka2.calkowitaDlugosc;
        dyst3[i] = frac * sciezka3.calkowitaDlugosc;
        krople2[i].setFillColor(sf::Color::Blue);
        krople3[i].setFillColor(sf::Color::Blue);
    }

    // Pompy
    {
        sf::Vector2f c12(
            rura1.getPosition().x + rura1.getSize().x / 2.f,
            rura1.getPosition().y + rura1.getSize().y / 2.f
        );
        pompa1 = Pompa(36.f, c12);
    }
    {
        sf::Vector2f b23(
            rura2_h1.getPosition().x + rura2_h1.getSize().x,
            rura2_h1.getPosition().y + rura2_h1.getSize().y / 2.f
        );
        pompa2 = Pompa(36.f, b23);
    }
    {
        sf::Vector2f b24(
            rura3_h1.getPosition().x + rura3_h1.getSize().x,
            rura3_h1.getPosition().y + rura3_h1.getSize().y / 2.f
        );
        pompa3 = Pompa(36.f, b24);
    }

    pokazPomoc = false;
    czyWyjsciePotwierdzone = false;
}

void Symulacja::obsluzZdarzenia() {
    sf::Event e;
    while (okno.pollEvent(e)) {
        if (e.type == sf::Event::Closed) {
            czyWyjsciePotwierdzone = true;
        }
        else if (e.type == sf::Event::KeyPressed) {
            if (!czyWyjsciePotwierdzone) {
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
                else if (e.key.code == sf::Keyboard::K) {
                    zapiszStanDoPliku("app_state.txt");
                }
                else if (e.key.code == sf::Keyboard::L) {
                    wczytajStanZPliku("app_state.txt");
                }

                if (aktualnyTryb == TrybPracy::AUTOMATYCZNY) {
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
                }

                if (aktualnyTryb == TrybPracy::RECZNY) {
                    if (e.key.code == sf::Keyboard::Q) predkosc0 += 10.f;
                    else if (e.key.code == sf::Keyboard::A) predkosc0 = std::max(0.f, predkosc0 - 10.f);
                    else if (e.key.code == sf::Keyboard::W) predkosc1 += 10.f;
                    else if (e.key.code == sf::Keyboard::S) predkosc1 = std::max(0.f, predkosc1 - 10.f);
                    else if (e.key.code == sf::Keyboard::E) predkosc2 += 10.f;
                    else if (e.key.code == sf::Keyboard::D) predkosc2 = std::max(0.f, predkosc2 - 10.f);
                    else if (e.key.code == sf::Keyboard::R) predkosc3 += 10.f;
                    else if (e.key.code == sf::Keyboard::F) predkosc3 = std::max(0.f, predkosc3 - 10.f);
                    else if (e.key.code == sf::Keyboard::H) {
                        grzanieBoileraReczne = !grzanieBoileraReczne;
                    }
                }
            }
            else {
                // Wyświetlane jest potwierdzenie wyjścia
                if (e.key.code == sf::Keyboard::T) {
                    zapiszStanPrzedZamknieciem = true;
                    okno.close();
                }
                else if (e.key.code == sf::Keyboard::N) {
                    czyWyjsciePotwierdzone = false;
                }
            }
        }
    }
}

void Symulacja::aktualizuj(float dt) {
    if (!czyWyjsciePotwierdzone) {
        if (!pokazWidokUproszczony) {
            // Aktualizacja położenia kropli
            for (int i = 0; i < liczbaKropli0; ++i) {
                dyst0[i] += predkosc0 * dt;
                if (dyst0[i] > sciezka0.calkowitaDlugosc) dyst0[i] -= sciezka0.calkowitaDlugosc;
            }
            for (int i = 0; i < liczbaKropli1; ++i) {
                dyst1[i] += predkosc1 * dt;
                if (dyst1[i] > sciezka1.calkowitaDlugosc) dyst1[i] -= sciezka1.calkowitaDlugosc;
            }
            for (int i = 0; i < liczbaKropli; ++i) {
                dyst2[i] += predkosc2 * dt;
                if (dyst2[i] > sciezka2.calkowitaDlugosc) dyst2[i] -= sciezka2.calkowitaDlugosc;

                dyst3[i] += predkosc3 * dt;
                if (dyst3[i] > sciezka3.calkowitaDlugosc) dyst3[i] -= sciezka3.calkowitaDlugosc;
            }
            // Obroty pomp
            pompa1.obroc(predkosc1 * dt);
            pompa2.obroc(predkosc2 * dt);
            pompa3.obroc(predkosc3 * dt);
        }

        // Logika w zależności od trybu
        if (aktualnyTryb == TrybPracy::AUTOMATYCZNY) {
            aktualizujGrzanieAutomatyczne(dt);
        }
        else {
            // Tryb ręczny
            if (grzanieBoileraReczne) {
                float docelowaTemp = 90.f;
                float roznicaT = docelowaTemp - zbior2.temperatura;
                float wspolczynnikGrzania = 2.0f;
                float lokalnaHistereza = 0.5f;
                if (std::fabs(roznicaT) > lokalnaHistereza) {
                    zbior2.temperatura += roznicaT * wspolczynnikGrzania * dt;
                }
            }

            float out3 = odplyw3 * dt;
            zbior3.pobierzWode(out3);
            float out4 = odplyw4 * dt;
            zbior4.pobierzWode(out4);

            float tempOtoczenia = 15.f;
            float wspChlodz = 0.01f;
            zbior2.temperatura -= (zbior2.temperatura - tempOtoczenia) * wspChlodz * dt;
            zbior3.temperatura -= (zbior3.temperatura - tempOtoczenia) * wspChlodz * dt;
            zbior4.temperatura -= (zbior4.temperatura - tempOtoczenia) * wspChlodz * dt;

            if (predkosc1 > 0.f) {
                float dtLit1 = predkosc1 * dt;
                float pobrane = zbior1.pobierzWode(dtLit1);
                zbior2.dodajWode(pobrane, zbior1.temperatura);
            }
            if (predkosc2 > 0.f) {
                float dtLit2 = predkosc2 * dt;
                float pobrane = zbior2.pobierzWode(dtLit2);
                zbior3.dodajWode(pobrane, zbior2.temperatura);
            }
            if (predkosc3 > 0.f) {
                float dtLit3 = predkosc3 * dt;
                float pobrane = zbior2.pobierzWode(dtLit3);
                zbior4.dodajWode(pobrane, zbior2.temperatura);
            }
        }
    }
}

void Symulacja::aktualizujGrzanieAutomatyczne(float dt) {
    float wspGrzania = 2.0f;
    float wspChlodzenia = 0.01f;
    float tempDocelowaZb2 = 90.f;
    float lokalnaHistereza2 = 0.5f;
    float tempDocelowaZb3 = temperaturaZadana3;
    float tempDocelowaZb4 = temperaturaZadana4;
    float lokHistereza3 = 0.2f;
    float lokHistereza4 = 0.2f;

    float maksPredkosc3 = 300.f;
    float maksPredkosc4 = 300.f;
    float sumaPredkosci = maksPredkosc3 + maksPredkosc4;

    float odplywStaly3 = odplyw3 * 2.0f;
    float odplywStaly4 = odplyw4 * 2.0f;

    float progNiski = 30.f;
    float progWysoki = 90.f;

    static bool blokadaBoilera = false;
    static bool inicjacjaAuto = true;
    if (inicjacjaAuto) {
        zbior2.ustawProcentWody(50.f);
        zbior3.ustawProcentWody(30.f);
        zbior4.ustawProcentWody(30.f);
        zbior2.temperatura = zbior3.temperatura;
        inicjacjaAuto = false;
    }

    if (zbior2.procentWody <= progNiski) {
        blokadaBoilera = true;
    }

    if (blokadaBoilera) {
        if (zbior2.procentWody < progWysoki) {
            predkosc1 = sumaPredkosci;
            float dtLit = predkosc1 * dt;
            float pobrane = zbior1.pobierzWode(dtLit);
            // Prosty przykład mieszania temperatury (można dostosować)
            float calkObj = zbior2.procentWody + (pobrane / zbior2.procentWody) * 100.0f;
            zbior2.temperatura = (zbior2.temperatura * zbior2.procentWody + zbior1.temperatura * (pobrane / calkObj))
                / (zbior2.procentWody + (pobrane / 100.0f));

            zbior2.dodajWode(pobrane, zbior1.temperatura);
        }
        else {
            predkosc1 = 0.f;
        }

        float roznicaT = tempDocelowaZb2 - zbior2.temperatura;
        if (std::fabs(roznicaT) > lokalnaHistereza2) {
            zbior2.temperatura += roznicaT * wspGrzania * dt;
        }

        if (zbior2.procentWody >= progWysoki && zbior2.temperatura >= (tempDocelowaZb2 - lokalnaHistereza2)) {
            blokadaBoilera = false;
        }
    }
    else {
        float roznicaT = tempDocelowaZb2 - zbior2.temperatura;
        if (std::fabs(roznicaT) > lokalnaHistereza2) {
            zbior2.temperatura += roznicaT * wspGrzania * dt;
        }
        if (zbior2.procentWody >= progWysoki) {
            predkosc1 = 0.f;
        }
    }

    static bool pompa3On = false;
    static bool pompa4On = false;

    float dT3 = tempDocelowaZb3 - zbior3.temperatura;
    if (!pompa3On && dT3 > lokHistereza3) {
        pompa3On = true;
    }
    else if (pompa3On && dT3 < -lokHistereza3) {
        pompa3On = false;
    }
    predkosc2 = blokadaBoilera ? 0.f : (pompa3On ? maksPredkosc3 : 0.f);

    float dT4 = tempDocelowaZb4 - zbior4.temperatura;
    if (!pompa4On && dT4 > lokHistereza4) {
        pompa4On = true;
    }
    else if (pompa4On && dT4 < -lokHistereza4) {
        pompa4On = false;
    }
    predkosc3 = blokadaBoilera ? 0.f : (pompa4On ? maksPredkosc4 : 0.f);

    float dtLit3 = predkosc2 * dt;
    if (dtLit3 > 0.f) {
        float pobrane = zbior2.pobierzWode(dtLit3);
        float tmpZb3 = (zbior3.temperatura + zbior2.temperatura) / 1.2f;
        zbior3.dodajWode(pobrane, tmpZb3);
    }

    float dtLit4 = predkosc3 * dt;
    if (dtLit4 > 0.f) {
        float pobrane = zbior2.pobierzWode(dtLit4);
        float tmpZb4 = (zbior4.temperatura + zbior2.temperatura) / 1.2f;
        zbior4.dodajWode(pobrane, tmpZb4);
    }

    float out3 = odplywStaly3 * dt;
    zbior3.pobierzWode(out3);
    float out4 = odplywStaly4 * dt;
    zbior4.pobierzWode(out4);

    float tempOtoczenia = 15.f;
    zbior2.temperatura -= (zbior2.temperatura - tempOtoczenia) * wspChlodzenia * dt;
    zbior3.temperatura -= (zbior3.temperatura - tempOtoczenia) * wspChlodzenia * dt;
    zbior4.temperatura -= (zbior4.temperatura - tempOtoczenia) * wspChlodzenia * dt;
}

void Symulacja::renderuj() {
    okno.clear(sf::Color(50, 50, 50));

    // Jeśli widok uproszczony - rysujemy i koniec
    if (pokazWidokUproszczony) {
        nakladki.rysujWidokUproszczony(
            okno,
            zbior2.temperatura,    // boilerTemp
            zbior3.temperatura,    // tempZb3
            zbior4.temperatura,    // tempZb4
            temperaturaZadana3,    // zadana CO
            temperaturaZadana4     // zadana CWU
        );
        rysujEtykieteZapisu();  // Ewentualnie chcemy też rysować info o zapisie
        okno.display();
        return;
    }

    // Podświetlenie obramowania bojlera, jeśli grzanie w trybie RECZNYM jest aktywne
    if (aktualnyTryb == TrybPracy::RECZNY && grzanieBoileraReczne) {
        zbior2.ramka.setOutlineColor(sf::Color::Red);
    }
    else {
        zbior2.ramka.setOutlineColor(sf::Color::White);
    }

    // Ustawiamy pozycje kropli (dla widoku pełnego)
    for (int i = 0; i < liczbaKropli0; ++i) {
        sf::Vector2f p = sciezka0.wyznaczPozycje(dyst0[i]);
        krople0[i].setPosition(p - sf::Vector2f(7.f, 7.f));
    }
    for (int i = 0; i < liczbaKropli1; ++i) {
        sf::Vector2f p = sciezka1.wyznaczPozycje(dyst1[i]);
        krople1[i].setPosition(p - sf::Vector2f(7.f, 7.f));
    }
    for (int i = 0; i < liczbaKropli; ++i) {
        sf::Vector2f p = sciezka2.wyznaczPozycje(dyst2[i]);
        krople2[i].setPosition(p - sf::Vector2f(7.f, 7.f));

        sf::Vector2f q = sciezka3.wyznaczPozycje(dyst3[i]);
        krople3[i].setPosition(q - sf::Vector2f(7.f, 7.f));
    }

    // Rysowanie zbiorników
    zbior1.rysuj(okno);
    zbior2.rysuj(okno);
    zbior3.rysuj(okno);
    zbior4.rysuj(okno);

    // Rysowanie rur
    okno.draw(rura0);
    okno.draw(rura1);
    okno.draw(rura2_h1);
    okno.draw(rura2_v);
    okno.draw(rura2_h2);
    okno.draw(rura3_h1);
    okno.draw(rura3_v);
    okno.draw(rura3_h2);

    // Rysowanie kropli
    for (auto& c : krople0) okno.draw(c);
    for (auto& c : krople1) okno.draw(c);
    for (auto& c : krople2) okno.draw(c);
    for (auto& c : krople3) okno.draw(c);

    // Rysowanie pomp
    pompa1.rysuj(okno);
    pompa2.rysuj(okno);
    pompa3.rysuj(okno);

    // Etykiety i inne UI
    rysujEtykietyZbiornikow();
    rysujKontrolkeTrybu();
    rysujEtykieteZapisu();

    // Nakładka pomocy
    if (pokazPomoc) {
        nakladki.rysujPomoc(okno);
    }
    // Nakładka potwierdzenia wyjścia
    if (czyWyjsciePotwierdzone) {
        nakladki.rysujNakladkeWyjscia(okno);
    }

    okno.display();
}

void Symulacja::rysujKontrolkeTrybu() {
    if (czcionka.getInfo().family.empty()) return;

    sf::RectangleShape panel(sf::Vector2f(300.f, 40.f));
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    float px = (okno.getSize().x / 2.f) - 150.f;
    float py = okno.getSize().y - 40.f;
    panel.setPosition(px, py);
    okno.draw(panel);

    sf::Text txtTryb;
    txtTryb.setFont(czcionka);
    txtTryb.setCharacterSize(16);
    txtTryb.setFillColor(sf::Color::White);

    std::string trybStr = (aktualnyTryb == TrybPracy::AUTOMATYCZNY) ? "AUTO" : "RECZNY";
    txtTryb.setString("Tryb: " + trybStr);
    txtTryb.setPosition(px + 10.f, py + 8.f);
    okno.draw(txtTryb);

    sf::Text txtWidok;
    txtWidok.setFont(czcionka);
    txtWidok.setCharacterSize(16);
    txtWidok.setFillColor(sf::Color::White);

    std::string wStr = pokazWidokUproszczony ? "Uproszczony" : "Pelny";
    txtWidok.setString("Widok: " + wStr);
    txtWidok.setPosition(px + 150.f, py + 8.f);
    okno.draw(txtWidok);
}

void Symulacja::rysujEtykieteZapisu() {
    if (czcionka.getInfo().family.empty()) return;
    sf::Text txt;
    txt.setFont(czcionka);
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color::Green);

    std::string info = "[K] - Zapisz stan do pliku   [L] - Wczytaj stan";
    txt.setString(info);

    float px = 10.f;
    float py = okno.getSize().y - 22.f;
    txt.setPosition(px, py);

    okno.draw(txt);
}

void Symulacja::rysujEtykietyZbiornikow() {
    if (czcionka.getInfo().family.empty()) return;

    auto rysZbiornikLabel = [&](Zbiornik& zb, const std::string& nazwa) {
        sf::Text t;
        t.setFont(czcionka);
        t.setCharacterSize(16);
        t.setFillColor(sf::Color::White);

        int iT = (int)zb.temperatura;
        int iP = (int)zb.procentWody;
        std::string str = nazwa + ": " + std::to_string(iT) + "C, " + std::to_string(iP) + "%";
        t.setString(str);

        float x = zb.ramka.getPosition().x + zb.ramka.getSize().x / 2.f;
        float y = zb.ramka.getPosition().y + zb.ramka.getSize().y + 5.f;
        t.setPosition(x, y);
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.width / 2.f, 0.f);

        okno.draw(t);
        };

    rysZbiornikLabel(zbior1, "Bufor");
    rysZbiornikLabel(zbior2, "Bojler");
    rysZbiornikLabel(zbior3, "CO");
    rysZbiornikLabel(zbior4, "CWU");

    auto stworzLinie = [&](const std::string& s, float offY) {
        sf::Text t;
        t.setFont(czcionka);
        t.setCharacterSize(18);
        t.setFillColor(sf::Color::Cyan);
        t.setString(s);
        sf::FloatRect r = t.getLocalBounds();
        float xx = 1000.f / 2.f;
        float yy = 10.f + offY;
        t.setOrigin(r.width / 2.f, 0.f);
        t.setPosition(xx, yy);
        return t;
        };

    int tz3 = (int)temperaturaZadana3;
    int tz4 = (int)temperaturaZadana4;
    int s0 = (int)predkosc0;
    int s1 = (int)predkosc1;
    int s2 = (int)predkosc2;
    int s3 = (int)predkosc3;

    std::string lin1 = "Temp zadana CO = " + std::to_string(tz3) + "C,  Temp zadana CWU = " + std::to_string(tz4) + "C";
    sf::Text l1 = stworzLinie(lin1, 0.f);

    sf::Text l2 = stworzLinie("Doplyw wody do bufora = " + std::to_string(s0), 30.f);
    sf::Text l3 = stworzLinie("Obroty pompy uzupelniania = " + std::to_string(s1), 60.f);
    sf::Text l4 = stworzLinie("Obroty pompy CO = " + std::to_string(s2), 90.f);
    sf::Text l5 = stworzLinie("Obroty pompy CWU = " + std::to_string(s3), 120.f);

    okno.draw(l1);
    okno.draw(l2);
    okno.draw(l3);
    okno.draw(l4);
    okno.draw(l5);
}

// Zapisywanie stanu do pliku
void Symulacja::zapiszStanDoPliku(const std::string& nazwa) {
    std::ofstream plik(nazwa);
    if (!plik.is_open()) return;

    // Zapisujemy dane
    plik << (int)aktualnyTryb << "\n";
    plik << (pokazWidokUproszczony ? 1 : 0) << "\n";
    plik << (grzanieBoileraReczne ? 1 : 0) << "\n";

    plik << zbior2.procentWody << " " << zbior2.temperatura << "\n";
    plik << zbior3.procentWody << " " << zbior3.temperatura << "\n";
    plik << zbior4.procentWody << " " << zbior4.temperatura << "\n";

    plik.close();
}

// Wczytywanie stanu z pliku
void Symulacja::wczytajStanZPliku(const std::string& nazwa) {
    std::ifstream plik(nazwa);
    if (!plik.is_open()) return;

    int trybInt, uproszInt, heatInt;
    plik >> trybInt;
    plik >> uproszInt;
    plik >> heatInt;

    aktualnyTryb = (trybInt == 0) ? TrybPracy::AUTOMATYCZNY : TrybPracy::RECZNY;
    pokazWidokUproszczony = (uproszInt == 1);
    grzanieBoileraReczne = (heatInt == 1);

    float p2, t2, p3, t3, p4, t4;
    plik >> p2 >> t2;
    plik >> p3 >> t3;
    plik >> p4 >> t4;

    zbior2.ustawProcentWody(p2);
    zbior2.temperatura = t2;

    zbior3.ustawProcentWody(p3);
    zbior3.temperatura = t3;

    zbior4.ustawProcentWody(p4);
    zbior4.temperatura = t4;

    plik.close();
}

// Główna pętla gry / symulacji
void Symulacja::uruchom() {
    if (zaladujStan) {
        wczytajStanZPliku("app_state.txt");
        zaladujStan = false;
    }

    while (okno.isOpen()) {
        obsluzZdarzenia();
        float dt = zegar.restart().asSeconds();
        if (!czyWyjsciePotwierdzone) {
            aktualizuj(dt);
        }
        renderuj();
    }

    if (zapiszStanPrzedZamknieciem) {
        zapiszStanDoPliku("app_state.txt");
    }
}

// ---------------------------------------------------------
// Funkcja main()
// ---------------------------------------------------------
int main() {
    Symulacja sym;
    sym.uruchom();
    return 0;
}
