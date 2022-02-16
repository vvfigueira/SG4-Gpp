#ifndef StepAcao_h
#define StepAcao_h 1

// Includes da Simulação

#include "Detector.hh"
#include "EventoAcao.hh"

// Includes do Geant4

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

// Criação da classe 'StepAcao' derivada da classe 'G4UserSteppingAction' do Geant4

class StepAcao : public G4UserSteppingAction {
    public:

        // Construtor e aniquilador da classe

        StepAcao(const Detector*, EventoAcao*);
        virtual ~StepAcao();

        // Método que é executado a cada step

        virtual void UserSteppingAction(const G4Step*);
    private:

        // Referência ao detector do Geant4 e a classe EventoAcao

        const Detector* detec12;
        EventoAcao* event12;
};
#endif