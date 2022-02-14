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

typedef std::pair<double, double> EnergyRange_MeV;
typedef std::map<const std::string, EnergyRange_MeV> MapParticlesEnergy;

class GarfieldParticle {
 public:
  GarfieldParticle(std::string particleName, double ekin_eV, double time,
                   double x_cm, double y_cm, double z_cm, double dx, double dy,
                   double dz)
      : fParticleName(particleName),
        fEkin_MeV(ekin_eV / 1000000),
        fTime(time),
        fx_mm(10 * x_cm),
        fy_mm(10 * y_cm),
        fz_mm(10 * z_cm),
        fdx(dx),
        fdy(dy),
        fdz(dz) {}
  ~GarfieldParticle() {}

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
  std::string fParticleName;
  double fEkin_MeV, fTime, fx_mm, fy_mm, fz_mm, fdx, fdy, fdz;
};

class GarfieldDetector {
 public:
  static GarfieldDetector* GetInstance();
  static void Dispose();

  void InitializePhysics();

  void DoIt(std::string particleName, double ekin_MeV, double time, double x_cm,double y_cm, double z_cm, double dx, double dy, double dz);

  void AddParticleName(const std::string particleName, double ekin_min_MeV,double ekin_max_MeV, std::string program);
  bool FindParticleName(const std::string name,std::string program = "garfield");
  bool FindParticleNameEnergy(std::string name, double ekin_MeV,std::string program = "garfield");
  double GetMinEnergyMeVParticle(std::string name,std::string program = "garfield");
  double GetMaxEnergyMeVParticle(std::string name,std::string program = "garfield");
  void SetIonizationModel(std::string model, bool useDefaults = true);
  std::string GetIonizationModel();
  std::vector<GarfieldParticle*>* GetSecondaryParticles();
  void DeleteSecondaryParticles();
  
  inline void EnableCreateSecondariesInGeant4(bool flag) {createSecondariesInGeant4 = flag;}
  inline bool GetCreateSecondariesInGeant4() {return createSecondariesInGeant4;}
  inline double GetEnergyDeposit_MeV() { return fEnergyDeposit / 1000000; }
  inline double GetAvalancheSize() { return fAvalancheSize; }
  inline double GetGain() { return fGain; }
  
  inline void Clear() {
    fEnergyDeposit = 0;
    fAvalancheSize = 0;
    fGain = 0;
    nsum = 0;
  }

 private:
  GarfieldDetector();
  ~GarfieldDetector();

  std::string fIonizationModel;

  static GarfieldDetector* fGarfieldDetector;
  MapParticlesEnergy fMapParticlesEnergyGeant4;
  MapParticlesEnergy fMapParticlesEnergyGarfield;
  Garfield::MediumMagboltz* fMediumMagboltz;
  Garfield::Sensor* fSensor;
  Garfield::TrackHeed* fTrackHeed;
  Garfield::ComponentAnalyticField* fComponentAnalyticField;

  std::vector<GarfieldParticle*>* fSecondaryParticles;

  bool createSecondariesInGeant4;
  bool dado;
  double fEnergyDeposit;
  double fAvalancheSize;
  double fGain;
  int nsum;
  Garfield::ViewSignal signalView;
};
#endif