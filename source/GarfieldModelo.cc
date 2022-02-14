// Includes do C++

#include <iostream>

// Includes da Simulação

#include "GarfieldModelo.hh"

// Includes do Geant4

#include "G4Electron.hh"
#include "G4GDMLParser.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

GarfieldModelo::GarfieldModelo(G4String modelName, G4Region* envelope)
  : G4VFastSimulationModel(modelName, envelope) {
    fGarfieldDetector = GarfieldDetector::GetInstance();
    fGarfieldDetector->InitializePhysics();
}

GarfieldModelo::GarfieldModelo(G4String modelName)
  : G4VFastSimulationModel(modelName) {
    fGarfieldDetector = GarfieldDetector::GetInstance();
    fGarfieldDetector->InitializePhysics();
}

GarfieldModelo::~GarfieldModelo() {}

void GarfieldModelo::WriteGeometryToGDML(G4VPhysicalVolume* physicalVolume) {
  G4GDMLParser* parser = new G4GDMLParser();
  remove("garfieldGeometry.gdml");
  parser->Write("garfieldGeometry.gdml", physicalVolume, false);
  delete parser;
}

G4bool GarfieldModelo::IsApplicable(const G4ParticleDefinition& particleType) {
  G4String particleName = particleType.GetParticleName();
  if (fGarfieldDetector->FindParticleName(particleName, "garfield")) {
    return true;
  }
  return false;
}

G4bool GarfieldModelo::ModelTrigger(const G4FastTrack& fastTrack) {
  double ekin_MeV = fastTrack.GetPrimaryTrack()->GetKineticEnergy() / MeV;
  G4String particleName = fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  if (fGarfieldDetector->FindParticleNameEnergy(particleName, ekin_MeV, "garfield")) {
    return true;
  }
  return false;
}

void GarfieldModelo::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();
  G4ThreeVector localpos = fastTrack.GetPrimaryTrackLocalPosition();

  double ekin_MeV = fastTrack.GetPrimaryTrack()->GetKineticEnergy() / MeV;
  double globalTime = fastTrack.GetPrimaryTrack()->GetGlobalTime();

  G4String particleName =
    fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

  fastStep.KillPrimaryTrack();
  fastStep.SetPrimaryTrackPathLength(0.0);

  if (particleName == "kaon+") {
    particleName = "K+";
  } else if (particleName == "kaon-") {
    particleName = "K-";
  } else if (particleName == "anti_proton") {
    particleName = "anti-proton";
  }

  fGarfieldDetector->DoIt(particleName, ekin_MeV, globalTime, localpos.x() / CLHEP::cm,
    localpos.y() / CLHEP::cm, localpos.z() / CLHEP::cm, localdir.x(), localdir.y(), localdir.z());

  fastStep.SetTotalEnergyDeposited(fGarfieldDetector->GetEnergyDeposit_MeV());

  if (!fGarfieldDetector->GetCreateSecondariesInGeant4()) return;
  std::vector<GarfieldParticle*>* secondaryParticles =
    fGarfieldDetector->GetSecondaryParticles();

  if (secondaryParticles->empty()) return;
  fastStep.SetNumberOfSecondaryTracks(secondaryParticles->size());

  G4double totalEnergySecondaries_MeV = 0;

  for (auto it = secondaryParticles->begin(); it != secondaryParticles->end(); ++it) {
    G4double eKin_MeV = (*it)->getEkin_MeV();
    G4double time = (*it)->getTime();
    G4ThreeVector momentumDirection((*it)->getDX(), (*it)->getDY(), (*it)->getDZ());
    G4ThreeVector position((*it)->getX_mm(), (*it)->getY_mm(), (*it)->getZ_mm());
    if ((*it)->getParticleName() == "e-") {
      G4DynamicParticle particle(G4Electron::ElectronDefinition(), momentumDirection, eKin_MeV);
      fastStep.CreateSecondaryTrack(particle, position, time, true);
      totalEnergySecondaries_MeV += eKin_MeV;
    } else if ((*it)->getParticleName() == "gamma") {
       G4DynamicParticle particle(G4Gamma::GammaDefinition(), momentumDirection, eKin_MeV);
      fastStep.CreateSecondaryTrack(particle, position, time, true);
      totalEnergySecondaries_MeV += eKin_MeV;
    }
  }  
}