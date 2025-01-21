#pragma once
#ifndef ZAPISPLIKC_H
#define ZAPISPLIKC_H
#define _CRT_SECURE_NO_WARNINGS
#include <string>

class Symulacja;

class ZapisPlikC {
    friend class Symulacja;
    
    static void zapiszStan(const Symulacja& sym, const std::string& nazwaPliku);

    
    static void wczytajStan(Symulacja& sym, const std::string& nazwaPliku);
};

#endif 
