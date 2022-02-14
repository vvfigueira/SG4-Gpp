#ifndef RunAcao_h
#define RunAcao_h 1

// Includes do Geant4

#include "G4UserRunAction.hh"
#include "G4Run.hh"

class RunAcao : public G4UserRunAction
{
    public:
        RunAcao();
        virtual ~RunAcao();

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
};

#endif