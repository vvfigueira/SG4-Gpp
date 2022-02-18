// Includes da Simulação

#include "StepAcao.hh"
#include "Analise.hh"
#include "colors.hh"
#include "Constantes.hh"

// Construtor inicializando variáveis

StepAcao::StepAcao(const Detector* detec1, EventoAcao* event1)
    :G4UserSteppingAction(), detec12(detec1), event12(event1){
}

// Aniquilador

StepAcao::~StepAcao(){}

// Método executado em cada step

void StepAcao::UserSteppingAction(const G4Step* step){

    G4VPhysicalVolume* volume =
        step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    auto pname = step->GetTrack()->GetParticleDefinition()->GetParticleName();

    G4double edep = step->GetTotalEnergyDeposit();

    G4double stepLength = 0.;
    if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
        stepLength = step->GetStepLength();
    }

    if (volume == detec12->GetVFGas()) {
        event12->AddGas(edep);
    }

    if(volume == detec12->GetVFAnodo() && (pname == "e-" || pname == "electron")){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }

    if(volume == detec12->GetVFCatExt() && (pname == "e-" || pname == "electron")){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }

    if(volume == detec12->GetVFCatInt() && (pname == "e-" || pname == "electron")){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}