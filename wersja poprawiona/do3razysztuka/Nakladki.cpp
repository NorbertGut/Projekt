#include "Nakladki.h"
#include <sstream>

Nakladki::Nakladki(const sf::Font& f)
    : font(f)
{
}

void Nakladki::rysujPomoc(sf::RenderWindow& window)
{
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
        " - [K] Zapis do pliku\n"
        " - [L] Odczyt z pliku\n"
        " - W RECZNYM:\n"
        "   Q/A, W/S, E/D, R/F -> sterowanie przeplywami\n"
        "   H -> wlacz wylacz grzanie boilera\n",
        font,
        18
    );
    txt.setFillColor(sf::Color::White);
    txt.setPosition(210.f, 160.f);
    window.draw(txt);
}

void Nakladki::rysujWidokUproszczony(sf::RenderWindow& window,
    float boilerTemp,
    float tempZb3,
    float tempZb4,
    float temperaturaZad3,
    float temperaturaZad4
)
{
   
    sf::Vector2f rozmiar(500.f, 300.f);
    sf::RectangleShape bg(rozmiar);
    bg.setFillColor(sf::Color(0, 0, 0, 100));

  
    float bx = (window.getSize().x - rozmiar.x) * 0.5f;
    float by = (window.getSize().y - rozmiar.y) * 0.5f;
    bg.setPosition(bx, by);
    window.draw(bg);

   

    float verticalX = bx + 200.f;   
    float horizontalY = by + 150.f; 

  
    sf::RectangleShape pion(sf::Vector2f(3.f, rozmiar.y));
    pion.setFillColor(sf::Color::Black);
    pion.setPosition(verticalX, by);
    window.draw(pion);

    sf::RectangleShape poziom(sf::Vector2f(rozmiar.x - 200.f, 3.f));
    poziom.setFillColor(sf::Color::Black);
    poziom.setPosition(verticalX, horizontalY);
    window.draw(poziom);

    
    auto makeText = [&](const std::string& str, float x, float y, unsigned size = 16)
        {
            sf::Text t;
            t.setFont(font);
            t.setString(str);
            t.setCharacterSize(size);
            t.setFillColor(sf::Color::White);
            t.setPosition(x, y);
            return t;
        };

    
    {
        float leftX = bx + 20.f;   
        float topY = by + 20.f;  

       
        sf::Text boilerTytul = makeText("BOILER", leftX, topY, 18);
        window.draw(boilerTytul);

       
        topY += 40.f;
        int iBT = boilerTemp;
        std::string sTemp = "TEMP: " + std::to_string(iBT) + "C";
        sf::Text boilerTempTxt = makeText(sTemp, leftX, topY);
        window.draw(boilerTempTxt);

       
        topY += 40.f;
        std::string sZad = "ZADANA TEMP: 90C";
        sf::Text boilerZadTxt = makeText(sZad, leftX, topY);
        window.draw(boilerZadTxt);
    }

   
    {
        float sectX = verticalX + 20.f;  
        float sectY = by + 20.f;         

        sf::Text coTytul = makeText("CO", sectX, sectY, 18);
        window.draw(coTytul);

        sectY += 40.f;
        int iCT = tempZb3;
        std::string sCT = "TEMP: " + std::to_string(iCT) + "C";
        sf::Text coTempTxt = makeText(sCT, sectX, sectY);
        window.draw(coTempTxt);

        sectY += 40.f;
        int iCZ =temperaturaZad3;
        std::string sCZ = "ZADANA TEMP: " + std::to_string(iCZ) + "C";
        sf::Text coZadTxt = makeText(sCZ, sectX, sectY);
        window.draw(coZadTxt);

        
    }

    
    {
        float sectX = verticalX + 20.f;
        float sectY = horizontalY + 20.f; 

        sf::Text cwuTytul = makeText("CWU", sectX, sectY, 18);
        window.draw(cwuTytul);

        sectY += 40.f;
        int iCWT =tempZb4;
        std::string sCWT = "TEMP: " + std::to_string(iCWT) + "C";
        sf::Text cwuTempTxt = makeText(sCWT, sectX, sectY);
        window.draw(cwuTempTxt);

        sectY += 40.f;
        int iCWZ = temperaturaZad4;
        std::string sCWZ = "ZADANA TEMP: " + std::to_string(iCWZ) + "C";
        sf::Text cwuZadTxt = makeText(sCWZ, sectX, sectY);
        window.draw(cwuZadTxt);

        
    }

    
    
}

void Nakladki::rysujNakladkeWyjscia(sf::RenderWindow& window)
{
    sf::RectangleShape bg({ 400.f,200.f });
    bg.setFillColor(sf::Color(0, 0, 0, 180));
    bg.setPosition(300.f, 200.f);
    window.draw(bg);

    sf::Text txt("Czy na pewno wyjsc?\n[T]ak / [N]ie", font, 24);
    txt.setFillColor(sf::Color::White);
    txt.setPosition(320.f, 230.f);
    window.draw(txt);
}
