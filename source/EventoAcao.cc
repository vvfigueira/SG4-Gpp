// Includes da Simulação

#include "EventoAcao.hh"
#include "Analise.hh"
#include "GarfieldDetector.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

EventoAcao::EventoAcao(RunAcao* runAction)
    : G4UserEventAction(), fEnergyAbs(0.), fEnergyGas(0.), fTrackLAbs(0.){}

EventoAcao::~EventoAcao(){}

void EventoAcao::BeginOfEventAction(const G4Event* event){
    if(Dim::uGpp){
        fEnergyAbs = 0;
        fEnergyGas = 0;
        fTrackLAbs = 0;
        fAvalancheSize = 0;
        fGain = 0;

        GarfieldDetector* gpp = GarfieldDetector::GetInstance();
        gpp->Clear();
    }
}

void EventoAcao::EndOfEventAction(const G4Event* event){
    if(Dim::uGpp){
        GarfieldDetector* gpp = GarfieldDetector::GetInstance();

        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        // fEnergyGas += gpp->GetEnergyDeposit_MeV(); // ????????????
        fAvalancheSize = gpp->GetAvalancheSize();
        fGain = gpp->GetGain();

        if(fEnergyAbs!=0) analysisManager->FillH1(1, fEnergyAbs);
        if(fTrackLAbs!=0) analysisManager->FillH1(2, fTrackLAbs);
        if(fEnergyGas!=0) analysisManager->FillH1(3, fEnergyGas);
        if(fAvalancheSize!=0) analysisManager->FillH1(4, fAvalancheSize);
        if(fGain!=0) analysisManager->FillH1(5, fGain);

        if(fEnergyAbs!=0) analysisManager->FillNtupleDColumn(0, fEnergyAbs);
        if(fTrackLAbs!=0) analysisManager->FillNtupleDColumn(1, fTrackLAbs);
        if(fEnergyGas!=0) analysisManager->FillNtupleDColumn(2, fEnergyGas);
        if(fAvalancheSize!=0) analysisManager->FillNtupleDColumn(3, fAvalancheSize);
        if(fGain!=0) analysisManager->FillNtupleDColumn(4, fGain);

        G4int eventID = event->GetEventID();
        G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
        if ((printModulo > 0) && (eventID % printModulo == 0)) {
            if(fEnergyGas != 0 || fEnergyAbs !=0 || fTrackLAbs != 0 || fAvalancheSize != 0 || fGain != 0){
                G4cout << "---> Fim do Evento: " << eventID << G4endl;

                G4cout << "        Gas: Energia depositada: " << std::setw(7)
                    << G4BestUnit(fEnergyGas, "Energy")
                    << "       Tamanho da Avalanche: " << fAvalancheSize
                    << "       Ganho: " << fGain << G4endl;
            }
        }
    }
}