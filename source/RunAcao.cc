// Includes da Simulação

#include "RunAcao.hh"
#include "GeradorPrimarias.hh"
#include "Detector.hh"
#include "Analise.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

// Construtor inicializando histogramas

RunAcao::RunAcao()
    : G4UserRunAction(){
}

// Aniquilador deletando usos de memória

RunAcao::~RunAcao(){delete G4AnalysisManager::Instance();}

// Método executado no início de cada run

void RunAcao::BeginOfRunAction(const G4Run* run){
}

// Método executado no final de cada run

void RunAcao::EndOfRunAction(const G4Run* run){
}