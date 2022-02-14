#ifndef TrackAcao_h
#define TrackAcao_h 1

// Includes da Simulação

#include "EventoAcao.hh"

// Includes do Geant4

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class TrackAcao : public G4UserTrackingAction 
{
    public:
        TrackAcao(EventoAcao* eventoacao);
        ~TrackAcao() {};

        virtual void  PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);
};

#endif