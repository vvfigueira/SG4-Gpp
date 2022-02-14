#include "GeradorPrimarias.hh"

#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4Geantino.hh"

GeradorPrimarias::GeradorPrimarias()
    : G4VUserPrimaryGeneratorAction(), 
    mParticleGun( new G4ParticleGun( G4Geantino::Definition() ) ){}

GeradorPrimarias::~GeradorPrimarias(){ delete mParticleGun;}

void GeradorPrimarias::GeneratePrimaries(G4Event* anEvent)
{
    mParticleGun->GeneratePrimaryVertex(anEvent);   
}