#ifndef EventoAcao_h
#define EventoAcao_h 1

#include "RunAcao.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventoAcao : public G4UserEventAction
{
    public:
        
        EventoAcao(RunAcao* runAction);
        virtual ~EventoAcao();

        virtual void  BeginOfEventAction(const G4Event*);
        virtual void    EndOfEventAction(const G4Event*);

        inline void AddAbs(G4double de, G4double dl){fEnergyAbs += de;fTrackLAbs += dl;}
        inline void AddGas(G4double de){fEnergyGas+=de;}
   
    private:

        G4double fEnergyAbs;
        G4double fEnergyGas;
        G4double fTrackLAbs;
        G4double fAvalancheSize;
        G4double fGain;
};

#endif