#ifndef DetectorSensivel_h
#define DetectorSensivel_h 1

// Includes da Simulação

#include "Detector.hh"

// Includes do Geant4

#include "G4VSensitiveDetector.hh"

class Detector;

class DetectorSensivel : public G4VSensitiveDetector{
    public:

        DetectorSensivel(const G4String& name, G4double );
        ~DetectorSensivel();

        virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;
    private:

        G4String pName, electron, nVol, ouro, gas;
        G4double energia, pressao, campoInt, temperatura;
        G4String aNome;
        Detector* detec;
};
#endif