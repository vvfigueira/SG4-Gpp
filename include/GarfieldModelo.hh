#ifndef GarfieldModelo_h
#define GarfieldModelo_h 1

#include "GarfieldDetector.hh"

#include "G4VFastSimulationModel.hh"

class G4VPhysicalVolume;

class GarfieldModelo : public G4VFastSimulationModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  GarfieldModelo(G4String, G4Region*);
  GarfieldModelo(G4String);
  ~GarfieldModelo();

  void SetPhysics(GarfieldDetector* fGarfieldDetector);
  void WriteGeometryToGDML(G4VPhysicalVolume* physicalVolume);

  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);

 private:
  GarfieldDetector* fGarfieldDetector;
};

#endif