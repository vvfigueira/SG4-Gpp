#ifndef Constantes_h
#define Constantes_h 1

// Includes do Geant4

#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace Dim{

    static constexpr bool uGpp = true; // Usar Garfiedlpp?
    static constexpr bool ler = true; // Informações destacadas
    static constexpr bool modelo2 = false; // Deixar como 'false'

    static constexpr double Intensidade1 = 233.184*volt; // Intensidade entre o Cátodo de Alumínio e Cátodo de Nicrômio Externo
    static constexpr double Intensidade2 = 465.38*volt; // Intensidade entre o Cátodo de Nicrômo Externo e Interno
    static constexpr double Intensidade3 = 243.976*volt; // Intensidade entre o Cátodo Interno e o Ânodo de Ouro
    
    static constexpr double R = 62.3636*L*torr/(kelvin*mole); // Constante universal dos Gases Ideais
    static constexpr G4int variaveis = 8; // Número de variáveis para o Integrador de Caminho
    static constexpr G4double passoMin = 0.010*mm; // Passo mínimo utilizado pelo ChordFinder
    static constexpr G4int n = 60; // Número de fios nos Cátodos
    static constexpr double mP10 = 19.765946*g/mole; // Massa molar do P10, CH4 10% + Ar 90%
    static constexpr double mNe = 20.17*g/mole; // Massa molar do Neônio
    static constexpr double tamanhomundo = 1.*m; // Tamanho da Aresta do Mundo
    static constexpr double densidadenicr = 8.4*g/cm3; // Densidade do Nicrômio
    static constexpr double comprimento = 0.6*m; // Comprimento do Detector
    static constexpr double raio = 0.025*mm; // Raio dos fios de Nicrômio
    static constexpr double distcatint = 20.003*mm; // Distância, em relação ao Ânodo de Ouro, do Cátodo Interno
    static constexpr double distcatext = 96.003*mm; // Distância, em relação ao Ânodo de Ouro, do Cátodo Externo
    static constexpr double raiofioouro = raio/2; // Raio do Fio de Ouro
    static constexpr double raiointal = 10.5*cm; // Raio Interno Alumínio
    static constexpr double raioextal = 10.66*cm; // Raio Externo Alumínio
    static constexpr double pressaogas = 760.*torr; // Pressão do Gás
    static constexpr double potencialanodo = 2500*volt; // Potencial do Ânodo
    static constexpr double potencialcatint = 750*volt; // Potencial do Cátodo Interno
    static constexpr double potencialcatext = 20*volt; // Potencial do Cátodo Externo
    static constexpr double potencialal = 0*volt; // Potencial do Cátodo de Alumínio
    //static constexpr double intcamp = 12500*volt; // Intensidade base do Campo (Obsoleto)
    static constexpr double temperaturagas = 293.15*kelvin; // Temperatura do Gás
}

#endif