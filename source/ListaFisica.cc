// Includes da Simulação

#include "ListaFisica.hh"

// Includes do Geant4

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4OpticalPhysics.hh"

// Construtor inicializando listas de física

ListaFisica::ListaFisica(){
    //RegisterPhysics(new G4EmStandardPhysics());
    //RegisterPhysics(new G4EmLivermorePhysics());
    RegisterPhysics(new G4EmPenelopePhysics());
    RegisterPhysics(new G4EmStandardPhysics_option4());
    //RegisterPhysics(new G4OpticalPhysics());

}

// Aniquilador

ListaFisica::~ListaFisica(){}