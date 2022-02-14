#ifndef DetectorMens_h
#define DetectorMens_h 1

#include "Detector.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"

class Detector;

class DetectorMens : public G4UImessenger {
    public:
        DetectorMens(Detector*);
        ~DetectorMens();
        
        void SetNewValue(G4UIcommand*, G4String);

    private:
        Detector* detector;

        G4UIdirectory* detectorDir; 

        G4UIcmdWithADoubleAndUnit* SetGasPressureCmd, *SetGasTemperatureCmd, *SetFieldIntensityCmd;
        G4UIcmdWithoutParameter* GetGasPressureCmd, *GetGasTemperatureCmd, *GetFieldIntensityCmd;
        G4UIcommand* fIonizationModelCmd;
};
#endif