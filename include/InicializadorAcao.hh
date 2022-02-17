#ifndef InicializadorAcao_h
#define InicializadorAcao_h 1

// Includes da Simulação

#include "Detector.hh"
#include "EventoAcao.hh"
#include "RunAcao.hh"

// Includes do Geant4

#include "G4VUserActionInitialization.hh"

// Criação da classe 'InicializadorAcao' derivada da classe 
// 'G4VUserActionInitialization' do Geant4

class InicializadorAcao : public G4VUserActionInitialization
{
    public:

        // Construtor e aniquilador da classe

        InicializadorAcao(Detector* );
        virtual ~InicializadorAcao();

        // Métodos para construir classes adicionais do Geant4

        virtual void BuildForMaster() const;
        virtual void Build() const;
    private:

        // Declaração da variável do Detector do Geant4

        Detector* detec;
};

#endif