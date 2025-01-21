#include "Sterowanie.h"
#include "Symulacja.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Sterowanie::aktualizujAutomatycznie(Symulacja& sym, float dt)
{
    
    float docelowaZb2 = 90.f;
    float roznZb2 = docelowaZb2 - sym.zbior2.temperatura;
    float wspGrz2 = 2.0f;
    float lokHistereza2 = 0.5f;
    if (std::fabs(roznZb2) > lokHistereza2) {
        sym.zbior2.temperatura += roznZb2 * wspGrz2 * dt;
    }

   
    float out3 = sym.odplyw3 * 2.0f * dt;
    sym.zbior3.pobierzWode(out3);
    float out4 = sym.odplyw4 * 2.0f * dt;
    sym.zbior4.pobierzWode(out4);

   
    float dT3 = sym.temperaturaZadana3 - sym.zbior3.temperatura;
    float dT4 = sym.temperaturaZadana4 - sym.zbior4.temperatura;
    float lokHistereza3 = 0.2f;
    float lokHistereza4 = 0.2f;

    //CO
    static bool pompa3On = false;
    if (!pompa3On && dT3 > lokHistereza3) {
        pompa3On = true;
    }
    else if (pompa3On && dT3 < -lokHistereza3) {
        pompa3On = false;
    }
    sym.predkosc2 = (pompa3On ? 300.f : 0.f);

    //CWU
    static bool pompa4On = false;
    if (!pompa4On && dT4 > lokHistereza4) {
        pompa4On = true;
    }
    else if (pompa4On && dT4 < -lokHistereza4) {
        pompa4On = false;
    }
    sym.predkosc3 = (pompa4On ? 300.f : 0.f);

    //doplyw
    float progNiski = 30.f;
    if (sym.zbior2.procentWody < progNiski) {
        sym.predkosc1 = 400.f;
       
        float dtLit = sym.predkosc1 * dt;
        float pobrane = sym.zbior1.pobierzWode(dtLit);
        sym.zbior2.dodajWode(pobrane, sym.zbior1.temperatura);
    }
    else {
        sym.predkosc1 = 0.f;
    }

    
    if (sym.predkosc2 > 0.f) {
        float dtLit2 = sym.predkosc2 * dt;
        float pob = sym.zbior2.pobierzWode(dtLit2);
       
        sym.zbior3.dodajWode(pob, sym.zbior2.temperatura);
    }
    if (sym.predkosc3 > 0.f) {
        float dtLit3 = sym.predkosc3 * dt;
        float pob = sym.zbior2.pobierzWode(dtLit3);
        sym.zbior4.dodajWode(pob, sym.zbior2.temperatura);
    }

   
    float tempOtoczenia = 15.f;
    float wspChlod = 0.01f;
    sym.zbior2.temperatura -= (sym.zbior2.temperatura - tempOtoczenia) * wspChlod * dt;
    sym.zbior3.temperatura -= (sym.zbior3.temperatura - tempOtoczenia) * wspChlod * dt;
    sym.zbior4.temperatura -= (sym.zbior4.temperatura - tempOtoczenia) * wspChlod * dt;
}

void Sterowanie::aktualizujRecznie(Symulacja& sym, float dt)
{
   

    if (sym.grzanieBoileraReczne) {
        float docelowa = 90.f;
        float rozn = docelowa - sym.zbior2.temperatura;
        float wsp = 2.f;
        float lokalnaHistereza = 0.5f;
        if (std::fabs(rozn) > lokalnaHistereza) {
            sym.zbior2.temperatura += rozn * wsp * dt;
        }
    }

   
    float out3 = sym.odplyw3 * dt;
    sym.zbior3.pobierzWode(out3);
    float out4 = sym.odplyw4 * dt;
    sym.zbior4.pobierzWode(out4);

   
    float tempOtoczenia = 15.f;
    float wspChlod = 0.01f;
    sym.zbior2.temperatura -= (sym.zbior2.temperatura - tempOtoczenia) * wspChlod * dt;
    sym.zbior3.temperatura -= (sym.zbior3.temperatura - tempOtoczenia) * wspChlod * dt;
    sym.zbior4.temperatura -= (sym.zbior4.temperatura - tempOtoczenia) * wspChlod * dt;

    
    if (sym.predkosc1 > 0.f) {
        float dtLit1 = sym.predkosc1 * dt;
        float pob = sym.zbior1.pobierzWode(dtLit1);
        sym.zbior2.dodajWode(pob, sym.zbior1.temperatura);
    }
    if (sym.predkosc2 > 0.f) {
        float dtLit2 = sym.predkosc2 * dt;
        float pob = sym.zbior2.pobierzWode(dtLit2);
        sym.zbior3.dodajWode(pob, sym.zbior2.temperatura);
    }
    if (sym.predkosc3 > 0.f) {
        float dtLit3 = sym.predkosc3 * dt;
        float pob = sym.zbior2.pobierzWode(dtLit3);
        sym.zbior4.dodajWode(pob, sym.zbior2.temperatura);
    }
}
