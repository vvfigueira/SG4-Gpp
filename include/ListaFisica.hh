#ifndef ListaFisica_h
#define ListaFisica_h 1

// Includes do Geant4

#include "G4VModularPhysicsList.hh"

// Criação da classe 'ListaFísica' derivada da classe 
// 'G4VModularPhysicsList' do Geant4

class ListaFisica : public G4VModularPhysicsList{
    public:

        // Construtor e aniquilador da classe

        ListaFisica();
        ~ListaFisica();
};

#endif