// Includes do Programa

#include "Detector.hh"
#include "InicializadorAcao.hh"
#include "ListaFisica.hh"
#include "GarfieldListaFisica.hh"
#include "GarfieldDetector.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"

// Ajuda para uso

namespace prog {
    void PrintUsage() {
        G4cerr << " Modo de uso: " << G4endl;
        G4cerr << " ./build [-m Macro]" << G4endl;
    }
} 

int main(int argc,char** argv)
{
    G4String macro = "";
    
    switch (argc){
        case 3:
            macro = argv[2];
            break;
        
        default:
            prog::PrintUsage();
            return 1;
            break;
    }

    G4UIExecutive* ui = 0;
    if (!macro.size()) {
        ui = new G4UIExecutive(argc, argv);
    }

    auto detec = new Detector();

    G4RunManager* runManager = new G4RunManager;
    runManager->SetUserInitialization(detec);
    runManager->SetUserInitialization(new GarfieldListaFisica());
    runManager->SetUserInitialization(new InicializadorAcao(detec));

    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (macro.size()) {
        // Inicialização em modo batch
        UImanager->ApplyCommand("/control/execute " + macro);
    } else {
        // Inicialização em janela gráfica
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    if(macro.size()) GarfieldDetector::Dispose();
}