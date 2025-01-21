//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <iostream>
//#include <thread>
//#include <chrono>
//#include <random>
//
//// ================================
//// Klasa Symulacja – logika „minigry”
//// ================================
//class Symulacja {
//public:
//    Symulacja(sf::RenderWindow& window, sf::Font& font)
//        : okno(window), czcionka(font)
//    {
//    }
//
//    //===========================
//    //   MINIGRA WYJŚCIA
//    //===========================
//    bool miniGraWyjscie() {
//        // 1. Wyświetlenie nakładki z pytaniem o wybór liczby (1–6).
//        int wybranaLiczba = 0;
//        bool running = true;
//
//        while (running && okno.isOpen()) {
//            sf::Event event;
//            while (okno.pollEvent(event)) {
//                if (event.type == sf::Event::Closed) {
//                    // Jeśli ktoś zamknie okno z 'krzyżyka', wyjdźmy z pętli
//                    return false;
//                }
//                if (event.type == sf::Event::KeyPressed) {
//                    // Esc -> rezygnujemy z wyjścia
//                    if (event.key.code == sf::Keyboard::Escape) {
//                        return true;  // "nie zamykaj programu"
//                    }
//                    // Klawisze 1-6 -> wybór liczby
//                    if (event.key.code >= sf::Keyboard::Num1 &&
//                        event.key.code <= sf::Keyboard::Num6)
//                    {
//                        wybranaLiczba = (event.key.code - sf::Keyboard::Num1) + 1;
//                        running = false;
//                        break;
//                    }
//                }
//            }
//
//            // Rysuj scenę (czyli np. tło i instrukcje)
//            okno.clear(sf::Color::White);
//
//            // Rysuj półprzezroczyste tło
//            sf::RectangleShape bg(sf::Vector2f(400.f, 200.f));
//            bg.setFillColor(sf::Color(0, 0, 0, 180));
//            bg.setPosition(300.f, 200.f);
//            okno.draw(bg);
//
//            // Instrukcje tekstowe
//            sf::Text txt("Aby wyjsc, wybierz liczbe (1-6)\n[Esc] - Anuluj wyjscie",
//                czcionka, 24);
//            txt.setFillColor(sf::Color::White);
//            txt.setPosition(320.f, 230.f);
//            okno.draw(txt);
//
//            okno.display();
//        }
//
//        // Jeśli wybrano liczbę (1-6)
//        if (wybranaLiczba != 0) {
//            // 2. Animacja rzutu kostką
//            animujRzutKoscia();
//
//            // 3. Losujemy ostateczną wartość (1-6)
//            int wylosowana = losujOd1Do6();
//
//            std::cout << "Wybrana liczba: " << wybranaLiczba
//                << ", wylosowana: " << wylosowana << std::endl;
//
//            bool sukces = (wybranaLiczba > wylosowana);
//
//            // Komunikat z wynikiem
//            sf::Text wynikText;
//            wynikText.setFont(czcionka);
//            wynikText.setCharacterSize(32);
//            wynikText.setFillColor(sf::Color::Black);
//
//            if (sukces) {
//                wynikText.setString("Udało ci się wyjsc z gry!");
//            }
//            else {
//                wynikText.setString("Nie udalo sie wyjsc - wracasz do menu.");
//            }
//            wynikText.setPosition(220.f, 250.f);
//
//            //=========================================================
//            // Wyświetlamy wynik przez 2 sekundy (z możliwością zamknięcia okna)
//            //=========================================================
//            auto startTime = std::chrono::steady_clock::now();
//            auto waitTime = std::chrono::seconds(2);
//
//            while (okno.isOpen()) {
//                // Obsługa zdarzeń (np. zamknięcie okna)
//                sf::Event event;
//                while (okno.pollEvent(event)) {
//                    if (event.type == sf::Event::Closed) {
//                        okno.close();
//                        return false;
//                    }
//                }
//
//                // Czy minęły już 2 sekundy?
//                auto elapsed = std::chrono::steady_clock::now() - startTime;
//                if (elapsed >= waitTime) {
//                    break; // kończymy tę pętlę
//                }
//
//                // Rysowanie tła i komunikatu
//                okno.clear(sf::Color::White);
//
//                // Można dodać np. półprzezroczysty prostokąt:
//                sf::RectangleShape bg(sf::Vector2f(600.f, 100.f));
//                bg.setFillColor(sf::Color(0, 0, 0, 100));
//                bg.setPosition(100.f, 230.f);
//                okno.draw(bg);
//
//                okno.draw(wynikText);
//                okno.display();
//            }
//
//            // 4. Decyzja – zamknięcie lub powrót do menu
//            if (sukces) {
//                // Zamknij program
//                okno.close();
//                return false;
//            }
//            else {
//                // Zostaw otwarte okno, wróć do menu
//                return true;
//            }
//        }
//
//        // Jeżeli nic nie wybrano (np. okno zamknięte) - nie zamykamy programu
//        return true;
//    }
//
//private:
//    sf::RenderWindow& okno;
//    sf::Font& czcionka;
//
//    //===========================
//    // FUNKCJE POMOCNICZE
//    //===========================
//    int losujOd1Do6() {
//        static std::random_device rd;
//        static std::mt19937 gen(rd());
//        std::uniform_int_distribution<> dist(1, 6);
//        return dist(gen);
//    }
//
//    void animujRzutKoscia() {
//        int klatki = 15;
//        for (int i = 0; i < klatki; i++) {
//            // Losujemy tymczasową ściankę
//            int tmp = losujOd1Do6();
//
//            okno.clear(sf::Color::White);
//
//            sf::RectangleShape bg(sf::Vector2f(400.f, 200.f));
//            bg.setFillColor(sf::Color(0, 0, 0, 180));
//            bg.setPosition(300.f, 200.f);
//            okno.draw(bg);
//
//            sf::Text txt("Rzut kostka: " + std::to_string(tmp), czcionka, 40);
//            txt.setFillColor(sf::Color::White);
//            txt.setPosition(320.f, 230.f);
//            okno.draw(txt);
//
//            okno.display();
//            std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        }
//    }
//};
//
//// ================================
//// Funkcje do rysowania przycisków
//// ================================
//void rysujPrzycisk(sf::RenderWindow& okno, const sf::Font& font,
//    const sf::String& tekst, float x, float y,
//    sf::Vector2f rozmiar = { 200.f, 60.f })
//{
//    // Prostokąt tła (przycisk)
//    sf::RectangleShape przycisk(rozmiar);
//    przycisk.setFillColor(sf::Color(100, 100, 100));
//    przycisk.setPosition(x, y);
//    okno.draw(przycisk);
//
//    // Tekst na przycisku
//    sf::Text txt(tekst, font, 24);
//    txt.setFillColor(sf::Color::White);
//
//    // Wyśrodkowanie tekstu w przycisku (proste wyrównanie)
//    float offsetX = (rozmiar.x - txt.getLocalBounds().width) / 2.f;
//    float offsetY = (rozmiar.y - txt.getLocalBounds().height) / 2.f - 5.f;
//    txt.setPosition(x + offsetX, y + offsetY);
//
//    okno.draw(txt);
//}
//
//bool czyKlikNaPrzycisk(sf::Vector2i mousePos, float x, float y, sf::Vector2f rozmiar)
//{
//    if (mousePos.x >= x && mousePos.x <= x + rozmiar.x &&
//        mousePos.y >= y && mousePos.y <= y + rozmiar.y)
//    {
//        return true;
//    }
//    return false;
//}
//
//// ================================
////           Funkcja main
//// ================================
//int main() {
//    sf::RenderWindow okno(sf::VideoMode(800, 600), "Przykladowa Minigra (z GUI)");
//    okno.setFramerateLimit(60);
//
//    // Wczytanie czcionki (zmień ścieżkę na własną lub wstaw inną czcionkę)
//    sf::Font czcionka;
//    if (!czcionka.loadFromFile("arial.ttf")) {
//        std::cout << "Blad wczytywania czcionki. Upewnij sie, że plik arial.ttf istnieje.\n";
//    }
//
//    // Obiekt "Symulacja" - nasza logika minigry
//    Symulacja sym(okno, czcionka);
//
//    // Pozycje i rozmiary przykładowych przycisków
//    sf::Vector2f rozmiarPrzycisku = { 200.f, 60.f };
//    float xPrzycisk = 300.f;
//    float yStart = 200.f;
//    float yWyjscie = 300.f;
//
//    while (okno.isOpen()) {
//        sf::Event event;
//        while (okno.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                okno.close();
//            }
//            if (event.type == sf::Event::MouseButtonPressed &&
//                event.mouseButton.button == sf::Mouse::Left)
//            {
//                sf::Vector2i mousePos = sf::Mouse::getPosition(okno);
//
//                // Klik w przycisk "Start"?
//                if (czyKlikNaPrzycisk(mousePos, xPrzycisk, yStart, rozmiarPrzycisku)) {
//                    std::cout << "Rozpoczynamy (teoretycznie) gre!\n";
//                }
//
//                // Klik w przycisk "Wyjscie"?
//                if (czyKlikNaPrzycisk(mousePos, xPrzycisk, yWyjscie, rozmiarPrzycisku)) {
//                    // Uruchamiamy minigre wyjscia
//                    bool kontynuuj = sym.miniGraWyjscie();
//                    if (!okno.isOpen()) {
//                        // Okno zostało zamknięte w minigrze
//                        break;
//                    }
//                    if (!kontynuuj) {
//                        // Gdybyśmy chcieli coś jeszcze zrobić po powrocie...
//                        // Tutaj raczej nic, bo i tak wróciliśmy do menu.
//                    }
//                }
//            }
//        }
//
//        // Rysowanie menu głównego z białym tłem
//        okno.clear(sf::Color::White);
//
//        // Tytuł gry
//        sf::Text tytul("Moja Przykladowa Gra", czcionka, 36);
//        tytul.setFillColor(sf::Color::Black);
//        tytul.setPosition(200.f, 50.f);
//        okno.draw(tytul);
//
//        // Przycisk "Start"
//        rysujPrzycisk(okno, czcionka, "Start", xPrzycisk, yStart, rozmiarPrzycisku);
//        // Przycisk "Wyjscie"
//        rysujPrzycisk(okno, czcionka, "Wyjscie", xPrzycisk, yWyjscie, rozmiarPrzycisku);
//
//        okno.display();
//    }
//
//    return 0;
//}
