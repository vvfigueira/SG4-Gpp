#ifndef TrackAcao_h
#define TrackAcao_h 1

// Includes da Simulação

#include "EventoAcao.hh"

// Includes do Geant4

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

// Criação da classe 'TrackAcao' derivada da classe 'G4UserTrackingAction' do Geant4

class TrackAcao : public G4UserTrackingAction 
{
    public:

        // Construtor e aniquilador da classe

        TrackAcao(EventoAcao* eventoacao);
        ~TrackAcao();

        // Métodos que são executados antes e após cada Track

        virtual void  PreUserTrackingAction(const G4Track*);
        virtual void PostUserTrackingAction(const G4Track*);
};

#endif