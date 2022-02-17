// Includes da Simulação

#include "DetectorMens.hh"
#include "Detector.hh"
#include "GarfieldDetector.hh"

// Includes do Geant4

#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

// Criação de comandos do Garfield++

DetectorMens::DetectorMens(Detector* HPGeDet) : detector(HPGeDet) {

    fIonizationModelCmd =
        new G4UIcommand("/Detector/physics/ModIoni", this);
    fIonizationModelCmd->SetGuidance("Seleção do modelo de ionização do Garfield++");
    fIonizationModelCmd->SetGuidance("  e escolha se que utilizar as partículas");
    fIonizationModelCmd->SetGuidance("  e intervalos de energias padrões.");

    G4UIparameter* ionizationModelPrm =
        new G4UIparameter("ionizationModel", 's', false);
    ionizationModelPrm->SetGuidance(
        "Modelo de Ionização (1. PAIPhot, 2. PAI ou 3. Heed)");
    ionizationModelPrm->SetGuidance(
        "  1. Modelo PAIPhot no Geant4, elétrons delta são transportados pelo Heed");
    ionizationModelPrm->SetGuidance(
        "  2. Modelo PAI no Geant4, elétrons delta são transportados pelo Heed");
    ionizationModelPrm->SetGuidance("  3. Uso direto do Heed");
    fIonizationModelCmd->SetParameter(ionizationModelPrm);
    
    G4UIparameter* useDefaultsPrm = new G4UIparameter("Padrão", 'b', false);
    useDefaultsPrm->SetGuidance(
        "true para usar valores padrão, false para selecionar valores para cada partícula");
    fIonizationModelCmd->SetParameter(useDefaultsPrm);
    
    fIonizationModelCmd->AvailableForStates(G4State_PreInit);

}

// Aniquilador deleta diretório criado

DetectorMens::~DetectorMens() {
    delete detectorDir;
}

// Mudança dos valores por comandos de macro

void DetectorMens::SetNewValue(G4UIcommand* command, G4String newValues) {    
    
    if (command == fIonizationModelCmd) {
        GarfieldDetector* garfieldpp = GarfieldDetector::GetInstance();
        G4String modelName;
        G4bool useDefaults;
        std::istringstream is(newValues);
        is >> modelName >> std::boolalpha >> useDefaults;
        garfieldpp->SetIonizationModel(modelName, useDefaults);
    }
    
}