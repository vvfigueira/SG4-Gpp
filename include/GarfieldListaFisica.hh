#ifndef GarfieldListaFisica_h
#define GarfieldListaFisica_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class GarfieldListaFisica : public G4VModularPhysicsList {
 public:
  GarfieldListaFisica();
  virtual ~GarfieldListaFisica();
  virtual void SetCuts();
  virtual void ConstructParticle();
  virtual void ConstructProcess();

 protected:
  // these methods Construct physics processes and register them
  void AddParameterisation();
};

#endif