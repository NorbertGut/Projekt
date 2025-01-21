#include "Etykiety.h"
#include "Symulacja.h" 

Etykiety::Etykiety(const sf::Font& f)
    : font(f)
{
}

void Etykiety::rysujEtykietyZbiornikow(sf::RenderWindow& window, const Symulacja& sym)
{
    
    auto rysZbiornikLabel = [&](const Zbiornik& zb, const std::string& nazwa) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(16);
        t.setFillColor(sf::Color::White);

        int iT = static_cast<int>(zb.temperatura);
        int iP = static_cast<int>(zb.procentWody);
        std::string str = nazwa + ": " + std::to_string(iT) + "C, " + std::to_string(iP) + "%";
        t.setString(str);

        float x = zb.ramka.getPosition().x + zb.ramka.getSize().x / 2.f;
        float y = zb.ramka.getPosition().y + zb.ramka.getSize().y + 5.f;
        t.setPosition(x, y);

        
        sf::FloatRect bounds = t.getLocalBounds();
        t.setOrigin(bounds.width / 2.f, 0.f);

        window.draw(t);
        };

    
    rysZbiornikLabel(sym.zbior1, "Bufor");
    rysZbiornikLabel(sym.zbior2, "Bojler");
    rysZbiornikLabel(sym.zbior3, "CO");
    rysZbiornikLabel(sym.zbior4, "CWU");

   
    auto stworzLinie = [&](const std::string& s, float offY) {
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(18);
        t.setFillColor(sf::Color::Cyan);
        t.setString(s);

        sf::FloatRect r = t.getLocalBounds();
        float xx = window.getSize().x / 2.f;
        float yy = 10.f + offY;
        t.setOrigin(r.width / 2.f, 0.f);
        t.setPosition(xx, yy);

        return t;
        };

    int tz3 = sym.temperaturaZadana3;
    int tz4 = sym.temperaturaZadana4;
    int s0 = sym.predkosc0;
    int s1 = sym.predkosc1;
    int s2 = sym.predkosc2;
    int s3 = sym.predkosc3;

    std::string lin1 = "Temp zadana CO = " + std::to_string(tz3)
        + "C,  Temp zadana CWU = " + std::to_string(tz4) + "C";

    sf::Text l1 = stworzLinie(lin1, 0.f);
    sf::Text l2 = stworzLinie("Doplyw wody do bufora = " + std::to_string(s0), 30.f);
    sf::Text l3 = stworzLinie("Obroty pompy uzupelniania = " + std::to_string(s1), 60.f);
    sf::Text l4 = stworzLinie("Obroty pompy CO = " + std::to_string(s2), 90.f);
    sf::Text l5 = stworzLinie("Obroty pompy CWU = " + std::to_string(s3), 120.f);

    window.draw(l1);
    window.draw(l2);
    window.draw(l3);
    window.draw(l4);
    window.draw(l5);
}

void Etykiety::rysujKontrolkeTrybu(sf::RenderWindow& window, const Symulacja& sym)
{
   

    sf::RectangleShape panel(sf::Vector2f(300.f, 40.f));
    panel.setFillColor(sf::Color(0, 0, 0, 180));
    float px = (window.getSize().x / 2.f) - 150.f;
    float py = window.getSize().y - 40.f;
    panel.setPosition(px, py);
    window.draw(panel);

    sf::Text txtTryb;
    txtTryb.setFont(font);
    txtTryb.setCharacterSize(16);
    txtTryb.setFillColor(sf::Color::White);

    std::string trybStr = (sym.aktualnyTryb == TrybPracy::AUTOMATYCZNY) ? "AUTO" : "RECZNY";
    txtTryb.setString("Tryb: " + trybStr);
    txtTryb.setPosition(px + 10.f, py + 8.f);
    window.draw(txtTryb);

    sf::Text txtWidok;
    txtWidok.setFont(font);
    txtWidok.setCharacterSize(16);
    txtWidok.setFillColor(sf::Color::White);
    std::string wStr = (sym.pokazWidokUproszczony) ? "Uproszczony" : "Pelny";
    txtWidok.setString("Widok: " + wStr);
    txtWidok.setPosition(px + 150.f, py + 8.f);
    window.draw(txtWidok);
}

void Etykiety::rysujEtykieteZapisu(sf::RenderWindow& window)
{
   
    sf::Text txt;
    txt.setFont(font);
    txt.setCharacterSize(14);
    txt.setFillColor(sf::Color::Green);

    txt.setString("[K] - Zapisz stan do pliku   [L] - Wczytaj stan");

    float px = 10.f;
    float py = window.getSize().y - 22.f;
    txt.setPosition(px, py);

    window.draw(txt);
}
