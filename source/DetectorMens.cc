// Includes da Simulação

#include "DetectorMens.hh"
#include "Detector.hh"
#include "GarfieldDetector.hh"

// Includes do Geant4

#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

DetectorMens::DetectorMens(Detector* HPGeDet) : detector(HPGeDet) {

    detectorDir = new G4UIdirectory("/Detector/");
    detectorDir->SetGuidance("Controle de variáveis do Detector");

    SetGasPressureCmd =
        new G4UIcmdWithADoubleAndUnit("/Detector/DefGasPressao", this);

    SetGasPressureCmd->SetGuidance("Altera a Pressão do Gás Ativo.");
    SetGasPressureCmd->SetUnitCategory("Pressure");
    SetGasPressureCmd->SetDefaultValue(1*torr);
    SetGasPressureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    GetGasPressureCmd =
        new G4UIcmdWithoutParameter("/Detector/MostrarGasPressao", this);

    GetGasPressureCmd->SetGuidance("Mostra a Pressão atual do Gás Ativo.");
    GetGasPressureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    SetGasTemperatureCmd =
        new G4UIcmdWithADoubleAndUnit("/Detector/DefGasTemp", this);

    SetGasTemperatureCmd->SetGuidance("Altera a Temperatura do Gás Ativo.");
    SetGasTemperatureCmd->SetUnitCategory("Temperature");
    SetGasTemperatureCmd->SetDefaultValue(300*kelvin);
    SetGasTemperatureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    GetGasTemperatureCmd =
        new G4UIcmdWithoutParameter("/Detector/MostrarGasTemp", this);

    GetGasTemperatureCmd->SetGuidance("Mostra a Temperatura atual do Gás Ativo.");
    GetGasTemperatureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    SetFieldIntensityCmd =
        new G4UIcmdWithADoubleAndUnit("/Detector/DefCampoInt", this);

    SetFieldIntensityCmd->SetGuidance("Altera a Intensidade do Campo.");
    SetFieldIntensityCmd->SetUnitCategory("Electric potential");
    SetFieldIntensityCmd->SetDefaultValue(12500*volt);
    SetFieldIntensityCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    GetFieldIntensityCmd =
        new G4UIcmdWithoutParameter("/Detector/MostrarCampoInt", this);

    GetFieldIntensityCmd->SetGuidance("Mostra a Intensidade atual do Campo.");
    GetFieldIntensityCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fIonizationModelCmd =
        new G4UIcommand("/Detector/physics/setIonizationModel", this);
    fIonizationModelCmd->SetGuidance("Select ionization model for Garfield++");
    fIonizationModelCmd->SetGuidance("  and choose whether to use default particles");
    fIonizationModelCmd->SetGuidance("  and energy ranges for the chosen model");

    G4UIparameter* ionizationModelPrm =
        new G4UIparameter("ionizationModel", 's', false);
    ionizationModelPrm->SetGuidance(
        "ionization model (1. PAIPhot, 2. PAI or 3. Heed)");
    ionizationModelPrm->SetGuidance(
        "  1. PAIPhot model in Geant4, delta electrons transported by Heed");
    ionizationModelPrm->SetGuidance(
        "  2. PAI model in Geant4, delta electrons transported by Heed");
    ionizationModelPrm->SetGuidance("  3. Use directly Heed");
    fIonizationModelCmd->SetParameter(ionizationModelPrm);
    //
    G4UIparameter* useDefaultsPrm = new G4UIparameter("useDefaults", 'b', false);
    useDefaultsPrm->SetGuidance(
        "true to use default, false to manually choose particles and energies");
    fIonizationModelCmd->SetParameter(useDefaultsPrm);
    //
    fIonizationModelCmd->AvailableForStates(G4State_PreInit);

}

DetectorMens::~DetectorMens() {
    delete detectorDir;
}

void DetectorMens::SetNewValue(G4UIcommand* command, G4String newValues) {    
    
    if (command == SetGasPressureCmd){
        detector->SetGasPressure(SetGasPressureCmd->GetNewDoubleValue(newValues));
    }
    if(command == SetGasTemperatureCmd){
        detector->SetGasTemperature(SetGasTemperatureCmd->GetNewDoubleValue(newValues));
    }
    if (command == SetFieldIntensityCmd){
        detector->SetFieldIntensity(SetFieldIntensityCmd->GetNewDoubleValue(newValues));
    }
    if(command == GetGasPressureCmd){
        G4cout << "\nA Pressão é: " << G4BestUnit( detector->GetGasPressure(), "Pressure")<< "\n";
    }
    if(command == GetGasTemperatureCmd){
        G4cout << "\nA Temperatura é: " << G4BestUnit( detector->GetGasTemperature(), "Temperature")<< "\n";
    }
    if(command == GetFieldIntensityCmd){
        G4cout << "\nA Intensidade do Campo é: " << G4BestUnit( detector->GetFieldIntensity(), "Electric Potential")<< "\n";
    }
    if (command == fIonizationModelCmd) {
        GarfieldDetector* garfieldpp = GarfieldDetector::GetInstance();
        G4String modelName;
        G4bool useDefaults;
        std::istringstream is(newValues);
        is >> modelName >> std::boolalpha >> useDefaults;
        garfieldpp->SetIonizationModel(modelName, useDefaults);
    }
    
}