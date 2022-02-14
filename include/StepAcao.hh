#ifndef StepAcao_h
#define StepAcao_h 1

// Includes da Simulação

#include "Detector.hh"
#include "EventoAcao.hh"

// Includes do Geant4

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class StepAcao : public G4UserSteppingAction {
    public:

        StepAcao(const Detector*, EventoAcao*);
        virtual ~StepAcao();

        virtual void UserSteppingAction(const G4Step*);
    private:
        const Detector* detec12;
        EventoAcao* event12;
};
#endif