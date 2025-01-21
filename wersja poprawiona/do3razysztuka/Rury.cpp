#include "Rury.h"
#include <cmath>

void Rury::inicjalizuj(const Zbiornik& zb1,
    const Zbiornik& zb2,
    const Zbiornik& zb3,
    const Zbiornik& zb4)
{
    float grubosc = 15.f;
    float pol = grubosc / 2.f;

    //doplyw
    {
        float sx = zb1.ramka.getPosition().x + zb1.ramka.getSize().x / 2.f - pol;
        float sy = 0.f;
        float len = zb1.ramka.getPosition().y - sy;
        rura0.setSize({ grubosc, len });
        rura0.setPosition(sx, sy);
        rura0.setFillColor(sf::Color::Black);
    }

    //zb1->zb2
    {
        float sx = zb1.ramka.getPosition().x + zb1.ramka.getSize().x;
        float sy = zb1.ramka.getPosition().y + zb1.ramka.getSize().y / 2.f - pol;
        float ex = zb2.ramka.getPosition().x;
        float length = ex - sx;
        rura1.setSize({ length, grubosc });
        rura1.setPosition(sx, sy);
        rura1.setFillColor(sf::Color::Black);
    }

    //zb2->zb3
    {
        float sx = zb2.ramka.getPosition().x + zb2.ramka.getSize().x;
        float sy = zb2.ramka.getPosition().y + zb2.ramka.getSize().y * 0.25f - pol;
        float tx = zb3.ramka.getPosition().x - 100.f;
        float l1 = tx - sx;
        rura2_h1.setSize({ l1, grubosc });
        rura2_h1.setPosition(sx, sy);
        rura2_h1.setFillColor(sf::Color::Black);

        float ey = zb3.ramka.getPosition().y + zb3.ramka.getSize().y / 2.f - pol;
        float l2 = ey - sy;
        rura2_v.setSize({ grubosc, std::abs(l2) });
        rura2_v.setPosition(tx, std::min(sy, ey));
        rura2_v.setFillColor(sf::Color::Black);

        float hx2 = zb3.ramka.getPosition().x - tx;
        rura2_h2.setSize({ hx2, grubosc });
        rura2_h2.setPosition(tx, ey);
        rura2_h2.setFillColor(sf::Color::Black);
    }

    //zb2->zb4
    {
        float sx = zb2.ramka.getPosition().x + zb2.ramka.getSize().x;
        float sy = zb2.ramka.getPosition().y + zb2.ramka.getSize().y * 0.75f - pol;
        float tx = zb4.ramka.getPosition().x - 100.f;
        float hx1 = tx - sx;
        rura3_h1.setSize({ hx1, grubosc });
        rura3_h1.setPosition(sx, sy);
        rura3_h1.setFillColor(sf::Color::Black);

        float ey = zb4.ramka.getPosition().y + zb4.ramka.getSize().y / 2.f - pol;
        float vy = ey - sy;
        rura3_v.setSize({ grubosc, std::abs(vy) });
        rura3_v.setPosition(tx, std::min(sy, ey));
        rura3_v.setFillColor(sf::Color::Black);

        float hx2 = zb4.ramka.getPosition().x - tx;
        rura3_h2.setSize({ hx2, grubosc });
        rura3_h2.setPosition(tx, ey);
        rura3_h2.setFillColor(sf::Color::Black);
    }
}

void Rury::rysuj(sf::RenderWindow& okno)
{
    okno.draw(rura0);
    okno.draw(rura1);
    okno.draw(rura2_h1);
    okno.draw(rura2_v);
    okno.draw(rura2_h2);
    okno.draw(rura3_h1);
    okno.draw(rura3_v);
    okno.draw(rura3_h2);
}
