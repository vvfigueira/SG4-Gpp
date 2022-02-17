// Includes da Simulação

#include "TrackAcao.hh"

// Construtor

TrackAcao::TrackAcao(EventoAcao* eventoacao)
    :G4UserTrackingAction(){
}

// Aniquilador

TrackAcao::~TrackAcao(){}

// Método executado antes do Track

void TrackAcao::PreUserTrackingAction(const G4Track* track){}

// Método executado após o Track

void TrackAcao::PostUserTrackingAction(const G4Track* track){}