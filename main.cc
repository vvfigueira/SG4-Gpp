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
        G4cerr << " ./<nome_da_build> [-m macro ] [-u UIsession]" << G4endl;
    }
} 

int main(int argc,char** argv)
{

    if (argc > 7) {
        prog::PrintUsage();
        return 1;
    }

    G4String macro;
    G4String session;

    for (G4int i = 1; i < argc; i = i + 2) {
        if (G4String(argv[i]) == "-m")
            macro = argv[i + 1];
        else if (G4String(argv[i]) == "-u")
            session = argv[i + 1];
        else {
            prog::PrintUsage();
            return 1;
        }
    }

    G4UIExecutive* ui = 0;
    if (!macro.size()) {
        ui = new G4UIExecutive(argc, argv);
    }

    /*CLHEP::RanluxEngine defaultEngine( 1234567, 4 );
    G4Random::setTheEngine( &defaultEngine );
    G4int seed = time( NULL );
    G4Random::setTheSeed( seed );*/

    G4RunManager* runManager = new G4RunManager;

    auto detec = new Detector();

    runManager->SetUserInitialization(detec);
    if(Dim::uGpp){runManager->SetUserInitialization(new GarfieldListaFisica());}else{
        runManager->SetUserInitialization(new ListaFisica());
    }
    runManager->SetUserInitialization(new InicializadorAcao(detec));
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (macro.size()) {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    } else {
        // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    if(Dim::uGpp) GarfieldDetector::Dispose();
}