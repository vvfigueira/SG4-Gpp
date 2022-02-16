#ifndef EventoAcao_h
#define EventoAcao_h 1

// Includes da Simulação

#include "RunAcao.hh"

// Includes do Geant4

#include "G4UserEventAction.hh"
#include "globals.hh"

// Criação da classe 'EventoAcao' derivada da classe 'G4UserEventAction' do Geant4

class EventoAcao : public G4UserEventAction
{
    public:
        
        // Construtor e aniquilador da classe

        EventoAcao(RunAcao* runAction);
        virtual ~EventoAcao();

        // Métodos executados no início e final de cada evento

        virtual void  BeginOfEventAction(const G4Event*);
        virtual void    EndOfEventAction(const G4Event*);

        // Métodos para obtenção de energias depositadas

        inline void AddAbs(G4double de, G4double dl){fEnergyAbs += de;fTrackLAbs += dl;}
        inline void AddGas(G4double de){fEnergyGas+=de;}
   
    private:

        // Variáveis internas

        G4double fEnergyAbs;
        G4double fEnergyGas;
        G4double fTrackLAbs;
        G4double fAvalancheSize;
        G4double fGain;
};

#endif