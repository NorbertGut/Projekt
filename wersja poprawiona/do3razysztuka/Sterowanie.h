#ifndef STEROWANIE_H
#define STEROWANIE_H

class Symulacja;

class Sterowanie {
public:
    static void aktualizujAutomatycznie(Symulacja& sym, float dt);
    static void aktualizujRecznie(Symulacja& sym, float dt);
};

#endif 
