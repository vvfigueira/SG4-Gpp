#ifndef DetectorMens_h
#define DetectorMens_h 1

// Includes da Simulação

#include "Detector.hh"

// Includes do Geant4

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"

class Detector;

// Criação da classe 'DetectorMens' derivada da classe 'G4UImessenger' do Geant4

class DetectorMens : public G4UImessenger {
    public:

        // Construtor e aniquilador da classe

        DetectorMens(Detector*);
        ~DetectorMens();

        // Método para mudar valores por comandos
        
        void SetNewValue(G4UIcommand*, G4String);

    private:

        // Referência a classe 'Detector' criada

        Detector* detector;

        // Diretório principal de comandos

        G4UIdirectory* detectorDir; 

        // Tipos de comandos usados

        G4UIcmdWithADoubleAndUnit* SetGasPressureCmd, *SetGasTemperatureCmd, *SetFieldIntensityCmd;
        G4UIcmdWithoutParameter* GetGasPressureCmd, *GetGasTemperatureCmd, *GetFieldIntensityCmd;
        G4UIcommand* fIonizationModelCmd;
};
#endif