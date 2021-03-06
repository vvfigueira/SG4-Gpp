// Inlcudes do C++

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// Includes da Simulação

#include "DetectorSensivel.hh"
#include "DetectorMens.hh"
#include "Detector.hh"

// Includes do Geant4

#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4StepStatus.hh"

// Construção da classe inicializando valores nas variáveis

DetectorSensivel::DetectorSensivel(const G4String &name,G4double intensidade) 
    : G4VSensitiveDetector(name),
    campoInt(intensidade){
}

// Aniquilador da classe

DetectorSensivel::~DetectorSensivel(){}

// Método que registra hits

G4bool DetectorSensivel::ProcessHits(G4Step*aStep, G4TouchableHistory*ROhist){

    // Definições de variáveis

    pName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
    energia = aStep->GetPreStepPoint()->GetKineticEnergy();
    nVol = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    pressao = aStep->GetTrack()->GetVolume()->GetMotherLogical()->GetMaterial()->GetPressure();
    temperatura = aStep->GetTrack()->GetVolume()->GetMotherLogical()->GetMaterial()->GetTemperature();
    
    electron = "e-";
    ouro = "Ânodo de Ouro";
    gas = "Gás Ativo";

    // Truque para converter double para char*
    
    std::stringstream valorpress;
    valorpress << std::roundf(pressao/torr *100)/100;
    std::string presscorrig;
    valorpress >> presscorrig;

    std::stringstream valorint;
    valorint << std::roundf(campoInt/CLHEP::volt *100)/100;
    std::string intcorrig;
    valorint >> intcorrig;

    std::stringstream valortemp;
    valortemp << std::roundf(temperatura/CLHEP::kelvin *100)/100;
    std::string tempcorrig;
    valortemp >> tempcorrig;

    // Inicialização de arquivos para retirada de dados
    
    std::ofstream aFile (presscorrig+"torr.txt",std::ofstream::app);
    std::ofstream bFile (intcorrig+"V.txt", std::ofstream::app);
    std::ofstream cFile ("Histo2D.txt", std::ofstream::app);

    // Condições para retirada de dados de energias

    if (pName == "e-" && nVol == ouro){
        aFile << energia/CLHEP::eV << "\n";
        bFile << energia/CLHEP::eV << "\n";
        cFile << energia/CLHEP::eV << "\t"
        << temperatura/torr << "\n";
        aStep->GetPreStepPoint()->SetKineticEnergy(0*CLHEP::eV);
    };

    return 0;
}