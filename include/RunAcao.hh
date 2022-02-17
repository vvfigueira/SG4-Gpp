#ifndef RunAcao_h
#define RunAcao_h 1

// Includes do Geant4

#include "G4UserRunAction.hh"
#include "G4Run.hh"

// Criação da classe 'RunAcao' derivada da classe 'G4UserRunAction' do Geant4

class RunAcao : public G4UserRunAction
{
    public:
        
        // Construtor e aniquilador da classe

        RunAcao();
        virtual ~RunAcao();

        // Métodos que são executados no começo e final de cada run

        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
};

#endif