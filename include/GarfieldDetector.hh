#ifndef GarfieldDetector_h
#define GarfieldDetector_h 1

// Includes do C++

#include <iostream>
#include <map>
#include <vector>

// Includes do Garfield++

#include "Garfield/ComponentAnalyticField.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/TrackHeed.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/AvalancheMC.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/DriftLineRKF.hh"

// Definição da variável responsável pelo mapa partículas/intervalo de energias

typedef std::pair<double, double> EnergyRange_MeV;
typedef std::map<const std::string, EnergyRange_MeV> MapParticlesEnergy;

// Construção da classe responsável pelas partículas no Garfield++

class GarfieldParticle {
    public:

        // Construtor e aniquilador da classe

        GarfieldParticle(std::string particleName, double ekin_eV, double time,
            double x_cm, double y_cm, double z_cm, double dx, double dy,
            double dz): 
                fParticleName(particleName),
                fEkin_MeV(ekin_eV / 1000000),
                fTime(time),
                fx_mm(10 * x_cm),
                fy_mm(10 * y_cm),
                fz_mm(10 * z_cm),
                fdx(dx),
                fdy(dy),
                fdz(dz) {}
        ~GarfieldParticle() {}

        // Métodos internos do uso do Garfield++

        std::string getParticleName() { return fParticleName; }
        double getX_mm() { return fx_mm; }
        double getY_mm() { return fy_mm; }
        double getZ_mm() { return fz_mm; }
        double getEkin_MeV() { return fEkin_MeV; }
        double getTime() { return fTime; }
        double getDX() { return fdx; }
        double getDY() { return fdy; }
        double getDZ() { return fdz; }

    private:

        // Variáveis internas

        std::string fParticleName;
        double fEkin_MeV, fTime, fx_mm, fy_mm, fz_mm, fdx, fdy, fdz;
};

// Construção da classe responsável pelo detector do Garfield++

class GarfieldDetector {
    public:

        // Criador do Detector

        static GarfieldDetector* GetInstance();

        // Terminador do Detector

        static void Dispose();

        // Inicializa as características do detector especificadas em GarfieldDetector.cc

        void InitializePhysics();

        // Método chamado quando uma partícula penetra no gás, calcula as trajetórias
        // e retorna dados

        void DoIt(std::string particleName, double ekin_MeV, double time, double x_cm,double y_cm, double z_cm, double dx, double dy, double dz);

        // Métodos para configuração da integração

        void AddParticleName(const std::string particleName, double ekin_min_MeV,double ekin_max_MeV, std::string program);
        bool FindParticleName(const std::string name,std::string program = "garfield");
        bool FindParticleNameEnergy(std::string name, double ekin_MeV,std::string program = "garfield");
        double GetMinEnergyMeVParticle(std::string name,std::string program = "garfield");
        double GetMaxEnergyMeVParticle(std::string name,std::string program = "garfield");
        void SetIonizationModel(std::string model, bool useDefaults = true);
        std::string GetIonizationModel();
        std::vector<GarfieldParticle*>* GetSecondaryParticles();
        void DeleteSecondaryParticles();
        
        // Métodos adicionais opcionais

        inline void EnableCreateSecondariesInGeant4(bool flag) {createSecondariesInGeant4 = flag;}
        inline bool GetCreateSecondariesInGeant4() {return createSecondariesInGeant4;}
        inline double GetEnergyDeposit_MeV() { return fEnergyDeposit / 1000000; }
        inline double GetAvalancheSize() { return fAvalancheSize; }
        inline double GetGain() { return fGain; }
        
        // Reinicialização de variáveis

        inline void Clear() {
            fEnergyDeposit = 0;
            fAvalancheSize = 0;
            fGain = 0;
            nsum = 0;
        }

    private:

        // Constutor e aniquilador da classe

        GarfieldDetector();
        ~GarfieldDetector();

        // Modelo de ionização utilizado

        std::string fIonizationModel;

        // Variáveis internas do Garfield++

        static GarfieldDetector* fGarfieldDetector;
        MapParticlesEnergy fMapParticlesEnergyGeant4, fMapParticlesEnergyGarfield;
        Garfield::MediumMagboltz* fMediumMagboltz;
        Garfield::Sensor* fSensor;
        Garfield::TrackHeed* fTrackHeed;
        Garfield::AvalancheMC* fAvalMC;
        Garfield::AvalancheMicroscopic* fAvalmc;
        Garfield::DriftLineRKF* fDrift;
        Garfield::ComponentAnalyticField* fComponentAnalyticField;
        std::ofstream xFile, yFile, zFile, aFile, bFile, cFile;

        std::vector<GarfieldParticle*>* fSecondaryParticles;

        bool createSecondariesInGeant4;
        double fEnergyDeposit, fAvalancheSize, fGain;
        int nsum;
        Garfield::ViewSignal signalView;
};
#endif