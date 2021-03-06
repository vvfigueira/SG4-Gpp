// Includes da Simulação

#include "EventoAcao.hh"
#include "Analise.hh"
#include "GarfieldDetector.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

// Construtor inicializando valores de variáveis

EventoAcao::EventoAcao(RunAcao* runAction)
    : G4UserEventAction(), fEnergyAbs(0.), fEnergyGas(0.), fTrackLAbs(0.){
}

// Aniquilador

EventoAcao::~EventoAcao(){}

// Ações no começo de cada evento

void EventoAcao::BeginOfEventAction(const G4Event* event){

    fEnergyAbs = 0;
    fEnergyGas = 0;
    fTrackLAbs = 0;
    fAvalancheSize = 0;
    fGain = 0;

    GarfieldDetector* gpp = GarfieldDetector::GetInstance();
    gpp->Clear();
}

// Ações no final de cada evento

void EventoAcao::EndOfEventAction(const G4Event* event){
    
    GarfieldDetector* gpp = GarfieldDetector::GetInstance();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    fEnergyGas += gpp->GetEnergyDeposit_MeV();
    fAvalancheSize = gpp->GetAvalancheSize();
    fGain = gpp->GetGain();

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