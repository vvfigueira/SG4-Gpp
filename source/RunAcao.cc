#include "RunAcao.hh"
#include "GeradorPrimarias.hh"
#include "Detector.hh"
#include "Analise.hh"
#include "Constantes.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

RunAcao::RunAcao()
    : G4UserRunAction(){
        if(Dim::uGpp){
            G4RunManager::GetRunManager()->SetPrintProgress(1);

            G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
            G4cout << "Using " << analysisManager->GetType() << G4endl;

            analysisManager->SetVerboseLevel(1);
            analysisManager->SetFirstHistoId(1);

            analysisManager->CreateH1("1", "Edep in absorber", 100, 0., 800 * MeV);
            analysisManager->CreateH1("2", "trackL in absorber", 100, 0., 1 * m);
            analysisManager->CreateH1("3", "Deposito de energia no gas", 1000, 0., 100 * keV);

            analysisManager->CreateH1("4", "Tamanho da Avalanche no gas", 10000, 0, 10000);
            analysisManager->CreateH1("5", "Ganho", 1000, 0., 100);
            analysisManager->CreateH1("6", "Eletrons", 100., 0., 100*keV);
            analysisManager->CreateH3("1", "Posicao do Track", 200, -Dim::raioextal, Dim::raioextal,200,
                -Dim::raioextal, Dim::raioextal, 200, -Dim::comprimento, Dim::comprimento);

            analysisManager->CreateNtuple("Garfield", "Edep and TrackL");
            analysisManager->CreateNtupleDColumn("Eabs");
            analysisManager->CreateNtupleDColumn("Labs");
            analysisManager->CreateNtupleDColumn("EGas");
            analysisManager->CreateNtupleDColumn("TAvalanche");
            analysisManager->CreateNtupleDColumn("Ganho");
            analysisManager->CreateNtupleDColumn("Eletrons");
            analysisManager->FinishNtuple();
        }
}

RunAcao::~RunAcao(){delete G4AnalysisManager::Instance();}

void RunAcao::BeginOfRunAction(const G4Run* run){
    if(Dim::uGpp){
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile("Garfield");
    }
}

void RunAcao::EndOfRunAction(const G4Run* run){
    if(Dim::uGpp){
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

        if (analysisManager->GetH1(1)) {
            G4cout << G4endl << " ----> Estatisticas dos Histogramas ";
            if (isMaster) {
                G4cout << "para toda a Run " << G4endl << G4endl;
            } else {G4cout << "para o thread atual " << G4endl << G4endl;}

            G4cout << " EGas : Media = "
                << G4BestUnit(analysisManager->GetH1(3)->mean(), "Energy")
                << " Rms = "
                << G4BestUnit(analysisManager->GetH1(3)->rms(), "Energy") << G4endl;

            G4cout << " Tamanho da Avalanche : Media = " << analysisManager->GetH1(4)->mean()
                << " Rms = " << analysisManager->GetH1(4)->rms() << G4endl;

            G4cout << " Ganho : Media = " << analysisManager->GetH1(5)->mean()
                << " Rms = " << analysisManager->GetH1(5)->rms() << G4endl;
        }

        analysisManager->Write();
        analysisManager->CloseFile();
    }
}