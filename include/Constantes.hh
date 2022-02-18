#ifndef Constantes_h
#define Constantes_h 1

// Includes do Geant4

#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace Dim{

    static constexpr double raio = 0.025*mm; // Raio dos fios de Nicrômio
    static constexpr double distcatint = 20.003*mm; // Distância, em relação ao Ânodo de Ouro, do Cátodo Interno
    static constexpr double distcatext = 96.003*mm; // Distância, em relação ao Ânodo de Ouro, do Cátodo Externo
    static constexpr double raiofioouro = raio/2; // Raio do Fio de Ouro
    static constexpr double raiointal = 10.5*cm; // Raio Interno Alumínio
    static constexpr double raioextal = 10.66*cm; // Raio Externo Alumínio
    static constexpr double pressaogas = 1*atmosphere; // Pressão do Gás
    static constexpr double potencialanodo = 2500*volt; // Potencial do Ânodo
    static constexpr double potencialcatint = 750*volt; // Potencial do Cátodo Interno
    static constexpr double potencialcatext = 20*volt; // Potencial do Cátodo Externo
    static constexpr double potencialal = 0*volt; // Potencial do Cátodo de Alumínio
    static constexpr double temperaturagas = 293.15*kelvin; // Temperatura do Gás
}

#endif