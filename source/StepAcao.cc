// Includes da Simulação

#include "StepAcao.hh"
#include "Analise.hh"
#include "colors.hh"
#include "Constantes.hh"

StepAcao::StepAcao(const Detector* detec1, EventoAcao* event1)
    :G4UserSteppingAction(), detec12(detec1), event12(event1){}

StepAcao::~StepAcao(){}

void StepAcao::UserSteppingAction(const G4Step* step){
    if(Dim::uGpp){
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

        G4VPhysicalVolume* volume =
            step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        auto pname = step->GetTrack()->GetParticleDefinition()->GetParticleName();

        if(volume == detec12->GetVFTubo()){std::cout<<"PHOTON SAIU DO GÁS E ATINGIU ALUMÍNIO\n";}

        G4double edep = step->GetTotalEnergyDeposit();

        G4double stepLength = 0.;
        if (step->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) {
            stepLength = step->GetStepLength();
        }

        if (volume == detec12->GetVFGas()) {
            event12->AddGas(edep);
        }

        //if( volume == detec12->GetVFGas() && (pname == "e-" || pname == "electron") ){
        //    std::cout << "\n\n\t\t" << UNDL(BOLD(FRED("ELÉTRONS NO GÁS")))<<"\n\n";
        //    step->GetTrack()->SetTrackStatus(fStopAndKill);
        //}

        if(volume == detec12->GetVFAnodo() && (pname == "e-" || pname == "electron")){
            analysisManager->FillH1(6,step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            analysisManager->FillNtupleDColumn(6, step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }

        if(volume == detec12->GetVFCatExt() && (pname == "e-" || pname == "electron")){
            analysisManager->FillH1(6,step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            analysisManager->FillNtupleDColumn(6, step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }

        if(volume == detec12->GetVFCatInt() && (pname == "e-" || pname == "electron")){
            analysisManager->FillH1(6,step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            analysisManager->FillNtupleDColumn(6, step->GetPreStepPoint()->GetKineticEnergy()/CLHEP::eV);
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
    }
}