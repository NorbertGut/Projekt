#include "ObiektySymulacji.h"
#include <cmath>
#include <algorithm>


SciezkaKropli::SciezkaKropli()
    : calkowitaDlugosc(0.f)
{
}

SciezkaKropli::SciezkaKropli(const std::vector<sf::Vector2f>& pts)
    : punkty(pts), calkowitaDlugosc(0.f)
{
    if (punkty.size() < 2) return;
    dlugosci.resize(punkty.size() - 1);
    for (size_t i = 0; i < punkty.size() - 1; ++i) {
        float dx = punkty[i + 1].x - punkty[i].x;
        float dy = punkty[i + 1].y - punkty[i].y;
        float dist = std::sqrt(dx * dx + dy * dy);
        dlugosci[i] = dist;
        calkowitaDlugosc += dist;
    }
}

sf::Vector2f SciezkaKropli::wyznaczPozycje(float dystans) const
{
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

Zbiornik::Zbiornik(const sf::Vector2f& rozmiar, const sf::Vector2f& pozycja)
    : procentWody(0.f), temperatura(15.f)
{
    ramka.setSize(rozmiar);
    ramka.setPosition(pozycja);
    ramka.setFillColor(sf::Color::Transparent);
    ramka.setOutlineColor(sf::Color::White);
    ramka.setOutlineThickness(3.f);

    float szer = rozmiar.x - 6.f;
    float wys = rozmiar.y - 6.f;
    woda.setSize({ szer,0.f });
    woda.setFillColor(sf::Color::Blue);
    woda.setPosition(pozycja.x + 3.f, pozycja.y + rozmiar.y - 3.f);
}

void Zbiornik::ustawProcentWody(float p)
{
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

float Zbiornik::pobierzMaksPojemnosc() const
{
    float szer = ramka.getSize().x - 6.f;
    float wys = ramka.getSize().y - 6.f;
    return szer * wys;
}

float Zbiornik::pobierzAktualnaObjetosc() const
{
    return (procentWody / 100.f) * pobierzMaksPojemnosc();
}

void Zbiornik::dodajWode(float litry, float tIn)
{
    if (litry <= 0.f) return;
    float maxV = pobierzMaksPojemnosc();
    float curV = pobierzAktualnaObjetosc();
    float wolne = maxV - curV;
    float faktyczne = std::min(wolne, litry);
    if (faktyczne > 0.f) {
        float nowaTemp = (temperatura * curV + tIn * faktyczne) / (curV + faktyczne);
        temperatura = nowaTemp;
        float nowaV = curV + faktyczne;
        float p = (nowaV / maxV) * 100.f;
        ustawProcentWody(p);
    }
}

float Zbiornik::pobierzWode(float litry)
{
    float curV = pobierzAktualnaObjetosc();
    float faktyczne = std::min(curV, litry);
    float pozost = curV - faktyczne;
    float p = (pozost / pobierzMaksPojemnosc()) * 100.f;
    ustawProcentWody(p);
    return faktyczne;
}

void Zbiornik::rysuj(sf::RenderWindow& okno)
{
    okno.draw(ramka);
    okno.draw(woda);
}


Pompa::Pompa()
{
}

Pompa::Pompa(float srednica, sf::Vector2f pozycja)
{
    obudowa.setRadius(srednica / 2.f);
    obudowa.setOrigin(srednica / 2.f, srednica / 2.f);
    obudowa.setPosition(pozycja);
    obudowa.setFillColor(sf::Color(128, 128, 128));

    float grubosc = srednica * 0.2f;
    float dlu = srednica * 0.8f;

    ramie1.setSize({ grubosc,dlu });
    ramie1.setOrigin(grubosc / 2.f, dlu / 2.f);
    ramie1.setPosition(pozycja);
    ramie1.setFillColor(sf::Color::Black);

    ramie2.setSize({ dlu,grubosc });
    ramie2.setOrigin(dlu / 2.f, grubosc / 2.f);
    ramie2.setPosition(pozycja);
    ramie2.setFillColor(sf::Color::Black);
}

void Pompa::obroc(float kat)
{
    ramie1.rotate(kat);
    ramie2.rotate(kat);
}

void Pompa::rysuj(sf::RenderWindow& okno)
{
    okno.draw(obudowa);
    okno.draw(ramie1);
    okno.draw(ramie2);
}
