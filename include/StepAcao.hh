#include "Detector.hh"
#include "EventoAcao.hh"

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