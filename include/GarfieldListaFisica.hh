#ifndef GarfieldListaFisica_h
#define GarfieldListaFisica_h 1

// Includes do Geant4

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

// Criação da classe 'GarfieldListaFísica' derivada da classe 
// 'G4VModularPhysicsList' do Geant4

class GarfieldListaFisica : public G4VModularPhysicsList {
    public:

        // Construtor e aniquilador da classe 

        GarfieldListaFisica();
        virtual ~GarfieldListaFisica();

        // Métodos internos

        virtual void SetCuts();
        virtual void ConstructParticle();
        virtual void ConstructProcess();

    protected:
       
       // Método para criar a parametrização do Garfield++

        void AddParameterisation();
};

#endif