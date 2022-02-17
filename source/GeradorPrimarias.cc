// Includes da Simulação

#include "GeradorPrimarias.hh"

// Includes do Geant4

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Geantino.hh"

// Construtor inicializando gerador de partículas com geantino

GeradorPrimarias::GeradorPrimarias()
    : G4VUserPrimaryGeneratorAction(), 
    mParticleGun( new G4ParticleGun( G4Geantino::Definition() ) ){
}

// Aniquilador excluíndo gerador de partículas

GeradorPrimarias::~GeradorPrimarias(){ delete mParticleGun;}

// Método responsável por gerar partículas

void GeradorPrimarias::GeneratePrimaries(G4Event* anEvent)
{
    mParticleGun->GeneratePrimaryVertex(anEvent);   
}