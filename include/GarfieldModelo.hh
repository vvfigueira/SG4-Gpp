#ifndef GarfieldModelo_h
#define GarfieldModelo_h 1

// Includes da Simulação

#include "GarfieldDetector.hh"

// Includes do Geant4

#include "G4VFastSimulationModel.hh"

class G4VPhysicalVolume;

// Criação da classe 'GarfieldModelo' derivada da classe 
// 'G4VFastSimulationModel' do Geant4

class GarfieldModelo : public G4VFastSimulationModel {
    public:
        
        // Construtor e aniquilador da classe

        GarfieldModelo(G4String, G4Region*);
        GarfieldModelo(G4String);
        ~GarfieldModelo();

        // Métodos internos

        void SetPhysics(GarfieldDetector* fGarfieldDetector);
        void WriteGeometryToGDML(G4VPhysicalVolume* physicalVolume);

        virtual G4bool IsApplicable(const G4ParticleDefinition&);
        virtual G4bool ModelTrigger(const G4FastTrack&);
        virtual void DoIt(const G4FastTrack&, G4FastStep&);

    private:

        // Declaração da variável do detector do Garfield++

        GarfieldDetector* fGarfieldDetector;
};

#endif