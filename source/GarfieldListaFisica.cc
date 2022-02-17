// Includes da Simulação

#include "GarfieldListaFisica.hh"
#include "GarfieldDetector.hh"

// Includes do Geant4

#include "G4EmConfigurator.hh"
#include "G4FastSimulationManagerProcess.hh"
#include "G4LossTableManager.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "QGSP_BERT_HP.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

GarfieldListaFisica::GarfieldListaFisica() : G4VModularPhysicsList() {
    G4int verb = 0;
    SetVerboseLevel(verb);
    defaultCutValue = 1 * CLHEP::mm;
    RegisterPhysics(new G4EmStandardPhysics_option4());
    RegisterPhysics(new G4OpticalPhysics());
    /*QGSP_BERT_HP* physicsList = new QGSP_BERT_HP;
    for (G4int i = 0;; ++i) {
        G4VPhysicsConstructor* elem =
            const_cast<G4VPhysicsConstructor*>(physicsList->GetPhysics(i));
        if (elem == NULL) break;
        G4cout << "RegisterPhysics: " << elem->GetPhysicsName() << G4endl;
        RegisterPhysics(elem);
    }*/
}

GarfieldListaFisica::~GarfieldListaFisica() {}

void GarfieldListaFisica::AddParameterisation() {
    GarfieldDetector* GarfieldDetector = GarfieldDetector::GetInstance();

    std::string ionizationModel = GarfieldDetector->GetIonizationModel();

    auto fastSimProcess_garfield = new G4FastSimulationManagerProcess("G4FSMP_garfield");

    auto theParticleIterator = GetParticleIterator();
    theParticleIterator->reset();
    while ((*theParticleIterator)()) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4EmConfigurator* config = G4LossTableManager::Instance()->EmConfigurator();
        G4LossTableManager::Instance()->SetVerbose(1);

        auto particleName = particle->GetParticleName();
        if (GarfieldDetector->FindParticleName(particleName, "garfield")) {
            pmanager->AddDiscreteProcess(fastSimProcess_garfield);
        }

        if (GarfieldDetector->FindParticleName(particleName, "geant4")) {
            double eMin = MeV * GarfieldDetector->GetMinEnergyMeVParticle(
                particleName, "geant4");
            double eMax = MeV * GarfieldDetector->GetMaxEnergyMeVParticle(
                particleName, "geant4");
            if (ionizationModel == "PAI") {
                G4PAIModel* pai = new G4PAIModel(particle, "G4PAIModel");
                if (particleName == "e-" || particleName == "e+") {
                    config->SetExtraEmModel(particleName, "eIoni", pai,
                        "RGarfield", eMin, eMax, pai);
                } else if (particleName == "mu-" || particleName == "mu+") {
                    config->SetExtraEmModel(particleName, "muIoni", pai,
                        "RGarfield", eMin, eMax, pai);
                } else if (particleName == "proton" ||
                    particleName == "pi+" || particleName == "pi-") {
                    config->SetExtraEmModel(particleName, "hIoni", pai,
                        "RGarfield", eMin, eMax, pai);
                } else if (particleName == "alpha" || particleName == "He3" ||
                    particleName == "GenericIon") {
                    config->SetExtraEmModel(particleName, "ionIoni", pai,
                        "RGarfield", eMin, eMax, pai);
                }
            } else if (ionizationModel == "PAIPhot") {
                G4PAIPhotModel* paiPhot = new G4PAIPhotModel(particle, "G4PAIModel");
                if (particleName == "e-" || particleName == "e+") {
                    config->SetExtraEmModel(particleName, "eIoni", paiPhot,
                        "RGarfield", eMin, eMax, paiPhot);
                } else if (particleName == "mu-" || particleName == "mu+") {
                    config->SetExtraEmModel(particleName, "muIoni", paiPhot, 
                        "RGarfield", eMin, eMax, paiPhot);
                } else if (particleName == "proton" ||
                    particleName == "pi+" || particleName == "pi-") {
                    config->SetExtraEmModel(particleName, "hIoni", paiPhot,
                        "RGarfield", eMin, eMax, paiPhot);
                } else if (particleName == "alpha" || particleName == "He3" ||
                    particleName == "GenericIon") {
                    config->SetExtraEmModel(particleName, "ionIoni", paiPhot, 
                        "RGarfield", eMin, eMax, paiPhot);
                }
            }
        }
    }
}

void GarfieldListaFisica::SetCuts() {
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(100. * eV, 100. * TeV);

    SetCutsWithDefault();

    G4Region* region = G4RegionStore::GetInstance()->GetRegion("RGarfield");
    G4ProductionCuts* cuts = new G4ProductionCuts();
    cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("gamma"));
    cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("e-"));
    cuts->SetProductionCut(1 * um, G4ProductionCuts::GetIndex("e+"));
    if (region) {
        region->SetProductionCuts(cuts);
    }

    DumpCutValuesTable();
}

void GarfieldListaFisica::ConstructParticle() {
    G4VModularPhysicsList::ConstructParticle();
}

void GarfieldListaFisica::ConstructProcess() {
    G4VModularPhysicsList::ConstructProcess();
    AddParameterisation();
}