#ifndef Detector_h
#define Detector_h 1

// Includes da Simulação

#include "DetectorMens.hh"

// Includes do Geant4

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

static constexpr double torr = atmosphere/760.;

class DetectorMens;

// Criação da classe 'Detector' derivada da classe 'G4VUserDetectorConstruction' do Geant4

class Detector : public G4VUserDetectorConstruction
{
    public:

        // Construtor e aniquilador da classe

        Detector();
        virtual ~Detector();

        // Métodos para referenciar volumes físicos do Detector

        const G4VPhysicalVolume* GetVFGas() const {return VFGas;};
        const G4VPhysicalVolume* GetVFTubo() const {return VFTubo;};
        const G4VPhysicalVolume* GetVFCatExt() const {return VFCatExt;};
        const G4VPhysicalVolume* GetVFCatInt() const {return VFCatInt;};
        const G4VPhysicalVolume* GetVFAnodo() const {return VFAnodo;};

        // Métodos para obter/mudar parâmetros da simulação

        inline void SetGasPressure(G4double d){pressaogas = d;};
        inline double GetGasPressure(){return pressaogas;};

        inline void SetGasTemperature(G4double d){temperaturagas = d;};
        inline double GetGasTemperature(){return temperaturagas;};
        
        inline void SetFieldIntensity(G4double d){intcamp = d;};
        inline double GetFieldIntensity(){return intcamp;};

        // Método responsável pela construção de campos e de Detectores
        // Sensíveis

        virtual void ConstructSDandField();

        virtual G4VPhysicalVolume* Construct() override;

    private:

        // Método da construção de materiais usados

        void DefineMaterials();

        // Metodo da construção das geometrias usadas

        G4VPhysicalVolume* DefineVolumes();

        // Volumes físicos usados

        G4VPhysicalVolume* VFGas;
        G4VPhysicalVolume* VFAnodo;
        G4VPhysicalVolume* VFCatExt;
        G4VPhysicalVolume* VFCatInt;
        G4VPhysicalVolume* VFTubo;

        // Classe de comandos do Detector

        DetectorMens* detectormens;

        // Variáveis adicionais

        G4double intcamp, temperaturagas, pressaogas, DensidadeGas, 
            DensidadeGas2, DensidadeCO2, DensidadeCH4, DensidadeGas3,
            DensidadeNiCr;

};

#endif