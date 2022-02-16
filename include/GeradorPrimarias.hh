#ifndef GeradorPrimarias_h
#define GeradorPrimarias_h 1

// Includes do Geant4

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"

// Criação da classe 'GeradorPrimarias' derivada da classe 
// 'G4VUserPrimaryGeneratorAction' do Geant4

class GeradorPrimarias : public G4VUserPrimaryGeneratorAction
{
    public:

        // Construtor e aniquilador da classe

        GeradorPrimarias();
        virtual ~GeradorPrimarias();

        // Método que cria partículas

        virtual void GeneratePrimaries(G4Event*);

    private:

        // Declaração da fonte de partículas

        G4ParticleGun*  mParticleGun;
};

#endif