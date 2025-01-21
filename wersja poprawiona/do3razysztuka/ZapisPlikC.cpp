#define _CRT_SECURE_NO_WARNINGS

#include "ZapisPlikC.h"
#include "Symulacja.h"  

#include <cstdio>       
#include <sstream>      

void ZapisPlikC::zapiszStan(const Symulacja& sym, const std::string& nazwaPliku)
{
    
    FILE* f = std::fopen(nazwaPliku.c_str(), "w");
    if (!f) return; 

    
    std::fprintf(f, "%d\n", (int)sym.aktualnyTryb);
    std::fprintf(f, "%d\n", (sym.pokazWidokUproszczony ? 1 : 0));
    std::fprintf(f, "%d\n", (sym.grzanieBoileraReczne ? 1 : 0));

    
    std::fprintf(f, "%.2f %.2f\n", sym.zbior2.procentWody, sym.zbior2.temperatura);
    
    std::fprintf(f, "%.2f %.2f\n", sym.zbior3.procentWody, sym.zbior3.temperatura);
   
    std::fprintf(f, "%.2f %.2f\n", sym.zbior4.procentWody, sym.zbior4.temperatura);

    std::fclose(f);
}

void ZapisPlikC::wczytajStan(Symulacja& sym, const std::string& nazwaPliku)
{
    FILE* f = std::fopen(nazwaPliku.c_str(), "r");
    if (!f) return; 

    
    std::string fileContent;
    {
        char buf[256];
        while (std::fgets(buf, sizeof(buf), f)) {
            fileContent += buf;
        }
    }
    std::fclose(f);

   
    std::stringstream ss(fileContent);

    int tI, wI, hI;
    ss >> tI >> wI >> hI;
    sym.aktualnyTryb = (tI == 0) ? TrybPracy::AUTOMATYCZNY : TrybPracy::RECZNY;
    sym.pokazWidokUproszczony = (wI == 1);
    sym.grzanieBoileraReczne = (hI == 1);

    float p2, t2, p3, t3, p4, t4;
    ss >> p2 >> t2;
    ss >> p3 >> t3;
    ss >> p4 >> t4;

    sym.zbior2.ustawProcentWody(p2);
    sym.zbior2.temperatura = t2;
    sym.zbior3.ustawProcentWody(p3);
    sym.zbior3.temperatura = t3;
    sym.zbior4.ustawProcentWody(p4);
    sym.zbior4.temperatura = t4;
}
