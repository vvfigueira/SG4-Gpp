#ifndef InicializadorAcao_h
#define InicializadorAcao_h 1

#include "Detector.hh"
#include "EventoAcao.hh"
#include "RunAcao.hh"

#include "G4VUserActionInitialization.hh"

class InicializadorAcao : public G4VUserActionInitialization
{
    public:
        InicializadorAcao(Detector* );
        virtual ~InicializadorAcao();

        virtual void BuildForMaster() const;
        virtual void Build() const;
    private:
        Detector* detec;
};

#endif