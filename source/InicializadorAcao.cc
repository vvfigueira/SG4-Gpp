#include "InicializadorAcao.hh"
#include "GeradorPrimarias.hh"
#include "EventoAcao.hh"
#include "TrackAcao.hh"
#include "RunAcao.hh"
#include "StepAcao.hh"

InicializadorAcao::InicializadorAcao(Detector* detector)
 : G4VUserActionInitialization(), detec(detector){}

InicializadorAcao::~InicializadorAcao(){}

void InicializadorAcao::BuildForMaster() const
{}

void InicializadorAcao::Build() const
{
  
    SetUserAction(new GeradorPrimarias);

    RunAcao* runac = new RunAcao();
    SetUserAction(runac);

    EventoAcao* eventoacao = new EventoAcao(runac);
    SetUserAction(eventoacao);

    TrackAcao* trackacao = new TrackAcao(eventoacao);
    SetUserAction(trackacao);

    StepAcao* stepacao = new StepAcao(detec, eventoacao);
    SetUserAction(stepacao);

}