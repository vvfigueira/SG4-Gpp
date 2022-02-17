#ifndef DetectorSensivel_h
#define DetectorSensivel_h 1

// Includes da Simulação

#include "Detector.hh"

// Includes do Geant4

#include "G4VSensitiveDetector.hh"

class Detector;

// Criação da classe 'DetectorSensivel' derivada da classe 'G4VSensitiveDetector' do Geant4

class DetectorSensivel : public G4VSensitiveDetector{
    public:

        // Construtor e aniquilador da classe

        DetectorSensivel(const G4String& name, G4double );
        ~DetectorSensivel();

        // Método para verificar hits

        virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;
    private:

        // Referência a classe 'Detector' criada

        Detector* detec;

        // Variáveis internas usadas

        G4String pName, electron, nVol, ouro, gas, aNome;
        G4double energia, pressao, campoInt, temperatura;
};
#endif