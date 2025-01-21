#ifndef ANIMACJE_H
#define ANIMACJE_H

class Symulacja;

class Animacje {
public:
    
    static void aktualizujKrople(Symulacja& sym, float dt);

   
    static void rysujKrople(Symulacja& sym);

   
    static void aktualizujPompy(Symulacja& sym, float dt);

   
    static void rysujPompy(Symulacja& sym);
};

#endif 
