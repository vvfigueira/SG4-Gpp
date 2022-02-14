// Includes do C++

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

// Includes do ROOT

#include <TCanvas.h>
#include <TROOT.h>
#include "TApplication.h"

// Includes da Simulação

#include "GarfieldDetector.hh"
#include "Analise.hh"
#include "colors.hh"
#include "Constantes.hh"

// Includes do Geant4

#include "globals.hh"
#include "G4SystemOfUnits.hh"

// Includes do Garfield++

#include "Garfield/AvalancheMC.hh"
#include "Garfield/AvalancheMicroscopic.hh"
#include "Garfield/ViewSignal.hh"
#include "Garfield/DriftLineRKF.hh"
#include "Garfield/ViewField.hh"

GarfieldDetector* GarfieldDetector::fGarfieldDetector = 0;

GarfieldDetector* GarfieldDetector::GetInstance() {
  if (!fGarfieldDetector) {
    fGarfieldDetector = new GarfieldDetector();
  }
  return fGarfieldDetector;
}

void GarfieldDetector::Dispose() {
  delete fGarfieldDetector;
  fGarfieldDetector = 0;
}

GarfieldDetector::GarfieldDetector() {
  fSecondaryParticles = new std::vector<GarfieldParticle*>();
  fMediumMagboltz = 0;
  fSensor = 0;
  fComponentAnalyticField = 0;
  fTrackHeed = 0;
  createSecondariesInGeant4 = true;
  fIonizationModel = "PAIPhot";
}

GarfieldDetector::~GarfieldDetector() {
  DeleteSecondaryParticles();
  delete fSecondaryParticles;
  delete fMediumMagboltz;
  delete fSensor;
  delete fComponentAnalyticField;
  delete fTrackHeed;

  std::cout << "Deconstructor GarfieldDetector" << std::endl;
}

std::string GarfieldDetector::GetIonizationModel() { return fIonizationModel; }

void GarfieldDetector::SetIonizationModel(std::string model, bool useDefaults) {
  if (model != "PAIPhot" && model != "PAI" && model != "Heed") {
    std::cout << "Unknown ionization model " << model << std::endl;
    std::cout << "Using PAIPhot as default model!" << std::endl;
    model = "PAIPhot";
  }
  fIonizationModel = model;

  if (fIonizationModel == "PAIPhot" || fIonizationModel == "PAI") {
    if (useDefaults == true) {
      // Particle types and energies for which the G4FastSimulationModel with
      // Garfield++ is valid
      this->AddParticleName("e-", 1e-6, 1e-3, "garfield");
      this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");//--> Padrao
      //this->AddParticleName("gamma",1e+8,1e+10, "garfield");//-->Truque

      // Particle types and energies for which the PAI or PAIPhot model is valid
      this->AddParticleName("e-", 0, 1e+8, "geant4");
      this->AddParticleName("e+", 0, 1e+8, "geant4");
      this->AddParticleName("mu-", 0, 1e+8, "geant4");
      this->AddParticleName("mu+", 0, 1e+8, "geant4");
      this->AddParticleName("proton", 0, 1e+8, "geant4");
      this->AddParticleName("pi+", 0, 1e+8, "geant4");
      this->AddParticleName("pi-", 0, 1e+8, "geant4");
      this->AddParticleName("alpha", 0, 1e+8, "geant4");
      this->AddParticleName("He3", 0, 1e+8, "geant4");
      this->AddParticleName("GenericIon", 0, 1e+8, "geant4");
    }

  } else if (fIonizationModel == "Heed") {
    if (useDefaults == true) {
      // Particle types and energies for which the G4FastSimulationModel with
      // Garfield++ is valid
      this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");//-->Padrao
      //this->AddParticleName("gamma",1e+8,1e+10,"garfield");//-->Truque
      this->AddParticleName("e-", 6e-2, 1e+7, "garfield");
      this->AddParticleName("e+", 6e-2, 1e+7, "garfield");
      this->AddParticleName("mu-", 1e+1, 1e+8, "garfield");
      this->AddParticleName("mu+", 1e+1, 1e+8, "garfield");
      this->AddParticleName("pi-", 2e+1, 1e+8, "garfield");
      this->AddParticleName("pi+", 2e+1, 1e+8, "garfield");
      this->AddParticleName("kaon-", 1e+1, 1e+8, "garfield");
      this->AddParticleName("kaon+", 1e+1, 1e+8, "garfield");
      this->AddParticleName("proton", 9.e+1, 1e+8, "garfield");
      this->AddParticleName("anti_proton", 9.e+1, 1e+8, "garfield");
      this->AddParticleName("deuteron", 2.e+2, 1e+8, "garfield");
      this->AddParticleName("alpha", 4.e+2, 1e+8, "garfield");
    }
  }
}

void GarfieldDetector::AddParticleName(const std::string particleName,
                                      double ekin_min_MeV, double ekin_max_MeV,
                                      std::string program) {
  if (ekin_min_MeV >= ekin_max_MeV) {
    std::cout << "Ekin_min=" << ekin_min_MeV
              << " keV is larger than Ekin_max=" << ekin_max_MeV << " keV"
              << std::endl;
    return;
  }

  if (program == "garfield") {
    std::cout << "Garfield model (Heed) is applicable for G4Particle "
              << particleName << " between " << ekin_min_MeV << " MeV and "
              << ekin_max_MeV << " MeV" << std::endl;

    fMapParticlesEnergyGarfield.insert(std::make_pair(
        particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
  } else {
    std::cout << fIonizationModel << " is applicable for G4Particle "
              << particleName << " between " << ekin_min_MeV << " MeV and "
              << ekin_max_MeV << " MeV" << std::endl;
    fMapParticlesEnergyGeant4.insert(std::make_pair(
        particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
  }
}

bool GarfieldDetector::FindParticleName(std::string name, std::string program) {
  if (program == "garfield") {
    auto it = fMapParticlesEnergyGarfield.find(name);
    if (it != fMapParticlesEnergyGarfield.end()) return true;
  } else {
    auto it = fMapParticlesEnergyGeant4.find(name);
    if (it != fMapParticlesEnergyGeant4.end()) return true;
  }
  return false;
}

bool GarfieldDetector::FindParticleNameEnergy(std::string name, double ekin_MeV,
                                             std::string program) {
  if (program == "garfield") {
    auto it = fMapParticlesEnergyGarfield.find(name);
    if (it != fMapParticlesEnergyGarfield.end()) {
      EnergyRange_MeV range = it->second;
      if (range.first <= ekin_MeV && range.second >= ekin_MeV) {
        return true;
      }
    }
  } else {
    auto it = fMapParticlesEnergyGeant4.find(name);
    if (it != fMapParticlesEnergyGeant4.end()) {
      EnergyRange_MeV range = it->second;
      if (range.first <= ekin_MeV && range.second >= ekin_MeV) {
        return true;
      }
    }
  }
  return false;
}

double GarfieldDetector::GetMinEnergyMeVParticle(std::string name,
                                                std::string program) {
  if (program == "garfield") {
    auto it = fMapParticlesEnergyGarfield.find(name);
    if (it != fMapParticlesEnergyGarfield.end()) {
      EnergyRange_MeV range = it->second;
      return range.first;
    }
  } else {
    auto it = fMapParticlesEnergyGeant4.find(name);
    if (it != fMapParticlesEnergyGeant4.end()) {
      EnergyRange_MeV range = it->second;
      return range.first;
    }
  }
  return -1;
}

double GarfieldDetector::GetMaxEnergyMeVParticle(std::string name,
                                                std::string program) {
  if (program == "garfield") {
    auto it = fMapParticlesEnergyGarfield.find(name);
    if (it != fMapParticlesEnergyGarfield.end()) {
      EnergyRange_MeV range = it->second;
      return range.second;
    }
  } else {
    auto it = fMapParticlesEnergyGeant4.find(name);
    if (it != fMapParticlesEnergyGeant4.end()) {
      EnergyRange_MeV range = it->second;
      return range.second;
    }
  }
  return -1;
}

void GarfieldDetector::InitializePhysics() {
  // Define the gas mixture.
  //auto cfield = new TCanvas("field","",600., 600.);
  //auto field = new Garfield::ViewField();
  fMediumMagboltz = new Garfield::MediumMagboltz();
  fMediumMagboltz->SetComposition("ar", 70., "co2", 30.);
  fMediumMagboltz->SetTemperature(Dim::temperaturagas/kelvin);
  fMediumMagboltz->SetPressure(Dim::pressaogas/torr);
  fMediumMagboltz->Initialise(true);
  // Set the Penning transfer efficiency.
  const double rPenning = 0.57;
  const double lambdaPenning = 0.;
  fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  fMediumMagboltz->LoadGasFile("ar_70_co2_30_1000mbar.gas");

  fComponentAnalyticField = new Garfield::ComponentAnalyticField();
  fComponentAnalyticField->SetMedium(fMediumMagboltz);

  fComponentAnalyticField->AddWire(0., 0., 2 * Dim::raiofioouro/cm, Dim::potencialanodo/volt, "Anodo");

  for (int i = 0; i < Dim::n; i++) {
    std::stringstream valori;
    valori << i;
    std::string icorrig;
    valori >> icorrig;
    fComponentAnalyticField->AddWire(Dim::distcatint*cos(i*CLHEP::twopi/60.)/cm,
      Dim::distcatint*sin(i*CLHEP::twopi/60.)/cm,
      2*Dim::raio/cm,
      Dim::potencialcatint/volt,
      "CatInt"+icorrig);
  }

  for (int i = 0; i < Dim::n; i++) {
    std::stringstream valori;
    valori << i;
    std::string icorrig;
    valori >> icorrig;
    fComponentAnalyticField->AddWire(Dim::distcatext*cos(i*CLHEP::twopi/60.)/cm,
      Dim::distcatext*sin(i*CLHEP::twopi/60.)/cm,
      2*Dim::raio/cm,
      Dim::potencialcatext/volt,
      "CatExt"+icorrig);
  }

  // Add the tube.
  fComponentAnalyticField->AddTube((Dim::raioextal+Dim::raiointal)/(2*cm), 
    Dim::potencialal, 0, "Aluminio");

  fComponentAnalyticField->AddReadout("Anodo");

  //field->SetComponent(fComponentAnalyticField);
  //field->SetPlaneXY();
  //field->SetCanvas(cfield);
  //field->PlotContour();

  fSensor = new Garfield::Sensor();
  fSensor->AddComponent(fComponentAnalyticField);
  fSensor->SetArea();
  /*fSensor->AddElectrode(fComponentAnalyticField, "Anodo");
  fSensor->SetTimeWindow(-0.25,0.5,1000);
  if (!readTransferFunction(fSensor)) G4cout << "Erro, arquivo ´TransferFunction´ não providenciado.\n";
  fSensor->ClearSignal();*/

  fTrackHeed = new Garfield::TrackHeed();
  fTrackHeed->SetSensor(fSensor);
  fTrackHeed->EnableDeltaElectronTransport();
  fTrackHeed->EnableElectricField();
  fTrackHeed->EnablePhotonReabsorption();

  /*TCanvas* cS = nullptr;
  if (true) {
    cS = new TCanvas("cS", "", 600, 600);
    signalView.SetCanvas(cS);
    signalView.SetSensor(fSensor);
    signalView.SetLabelY("signal [fC]");
  } */
}

void GarfieldDetector::DoIt(std::string particleName, double ekin_MeV,
                           double time, double x_cm, double y_cm, double z_cm,
                           double dx, double dy, double dz) {
                            
  std::ofstream xFile ("AvaPEle.txt", std::ofstream::app);
  std::ofstream yFile ("AvaPTemp.txt", std::ofstream::app);
  std::ofstream zFile ("AvaTot.txt", std::ofstream::app);
  std::ofstream aFile ("EleEner.txt", std::ofstream::app);
  std::ofstream bFile ("ElePos.txt", std::ofstream::app);
  std::ofstream cFile ("EEnerPos.txt", std::ofstream::app);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  fEnergyDeposit = 0;
  DeleteSecondaryParticles();

  Garfield::AvalancheMC drift; // Usar sem modelo2
  // Garfield::DriftLineRKF drift; // Usar com modelo2
  drift.SetSensor(fSensor);
  //drift.SetGainFluctuationsPolya(0.4, 20000.); // Usar com modelo 2
  drift.SetDistanceSteps(1.e-4);// Usar sem modelo2
  Garfield::AvalancheMicroscopic avalanche;
  avalanche.SetSensor(fSensor);
  avalanche.DisableAvalancheSizeLimit();

  // Wire radius [cm]
  const double rWire = Dim::raiofioouro/cm;
  // Outer radius of the tube [cm]
  const double rTube = (Dim::raioextal+Dim::raiointal)/(2*cm);
  // Half-length of the tube [cm]
  const double lTube = Dim::comprimento/cm;

  double eKin_eV = ekin_MeV * 1e+6;

  double xc = 0., yc = 0., zc = 0., tc = 0.;
  // Number of electrons produced in a collision
  int nc = 0;
  // Energy loss in a collision
  double ec = 0.;
  // Dummy variable (not used at present)
  double extra = 0.;
  fEnergyDeposit = 0;
  if (fIonizationModel != "Heed" || particleName == "gamma") {
    if (particleName == "gamma") {
      fTrackHeed->TransportPhoton(x_cm, y_cm, z_cm, time, eKin_eV, dx, dy, dz,
                                  nc);
                                  std::cout << std::endl<<std::endl<< "\t"<<FGRN("Elétrons Gerados:")  << "\t" << nc 
              << std::endl;
    } else {
      fTrackHeed->TransportDeltaElectron(x_cm, y_cm, z_cm, time, eKin_eV, dx,
                                         dy, dz, nc);
      fEnergyDeposit = eKin_eV;
      std::cout << std::endl<<std::endl<< "\t"<<FGRN("Ganho:")  << "\t" << nc 
              << std::endl;
    }
    for (int cl = 0; cl < nc; cl++) {
      double xe, ye, ze, te;
      
      double ee, dxe, dye, dze;
      fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
      dado = false;
      if (ze < lTube && ze > -lTube && sqrt(xe * xe + ye * ye) < rTube) {
        nsum++;
        if (particleName == "gamma") {
          fEnergyDeposit += fTrackHeed->GetW();
        }
        analysisManager->FillH3(1, xe * 10, ye * 10, ze * 10);
        if (createSecondariesInGeant4) {
          double newTime = te;
          if (newTime < time) {
            newTime += time;
          }
          fSecondaryParticles->push_back(new GarfieldParticle(
              "e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
        }

        drift.DriftElectron(xe, ye, ze, te);

        double xe1, ye1, ze1, te1, ee1;//Usar sem modelo2
        double xe2, ye2, ze2, te2, ee2;

        int status;
        //drift.GetEndPoint(xe2, ye2, ze2, te2, status);//Usar com modelo 2
        drift.GetElectronEndpoint(0, xe1, ye1, ze1, te1, xe2, ye2, ze2, te2, status );//Usar sem modelo 2
        std::cout << "\n\n\t" << FGRN("Distância ao Ânodo:") << "\t" << sqrt(xe2*xe2 + ye2*ye2)/rWire << "\n\n\t";
        if(sqrt(xe2*xe2 + ye2*ye2) < 1.5*rWire){dado = true;}
        if(xe2*xe2 + ye2*ye2 <= 1000*rWire*rWire) std::cout << "\n\n\t\t" << UNDL(BOLD(FRED("ELÉTRONS NO ÂNODO")))<<"\n\n";                        

        if (0 < xe2 && xe2 < rWire) {
          xe2 += rWire;
        } else if (0 > xe2 && xe2 > -rWire) {
          xe2 += -rWire;
        } 
        if (0 < ye2 && ye2 < rWire) {
          ye2 += rWire;
        } else if (0 > ye2 && ye2 > -rWire) {
          ye2 += -rWire;
        }

        double e2 = 0.1;
        avalanche.AvalancheElectron(xe2, ye2, ze2, te2, e2, 0, 0, 0);

        int ne = 0, ni = 0;
        avalanche.GetAvalancheSize(ne, ni);
        int Ne = 0;
        Ne = avalanche.GetNumberOfElectronEndpoints();
        for(int k= 0;k<Ne;k++){
          avalanche.GetElectronEndpoint(k,xe1,ye1,ze1,te1,ee1,xe2,ye2,ze2,te2,ee2,status);
          double r = sqrt(xe2*xe2 + ye2*ye2 + ze2*ze2);
          aFile << ee2 << "\n";
          bFile << r/Dim::raiofioouro << "\n";
          cFile << ee2 << "\t" << r/Dim::raiofioouro<< "\n";
        }
        if( ne > 1)xFile << ne << "\n";
        if( ne > 1)yFile << ne << "\t" << te2-time << "\n";
        fAvalancheSize += ne;
        std::cout << "\t" <<FGRN("Avalanche:")<< "\t" << ne <<std::endl;
      }
    }
    if (fAvalancheSize > 1) zFile << fAvalancheSize <<"\n";
    std::cout << std::endl<<"\t"<<FGRN("Total:")<< "\t"<< fAvalancheSize<<std::endl;
  } else {
    fTrackHeed->SetParticle(particleName);
    fTrackHeed->SetKineticEnergy(eKin_eV);
    fTrackHeed->NewTrack(x_cm, y_cm, z_cm, time, dx, dy, dz);

    while (fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
      if(particleName == "e-" || particleName == "electron"){
        std::cout << std::endl<<std::endl<< "\t"<<FGRN("Ganho:")  << "\t" << nc << std::endl;
      }else{std::cout << std::endl<<std::endl<< "\t"<<FGRN("Elétrons Gerados:")  << "\t" << nc << std::endl;}      
      dado = false;
      if (zc < lTube && zc > -lTube && sqrt(xc * xc + yc * yc) < rTube) {
        nsum += nc;
        fEnergyDeposit += ec;
        for (int cl = 0; cl < nc; cl++) {
          double xe, ye, ze, te;
          double ee, dxe, dye, dze;
          fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
          if (ze < lTube && ze > -lTube && sqrt(xe * xe + ye * ye) < rTube) {
            analysisManager->FillH3(1, xe * 10, ye * 10, ze * 10);
            if (createSecondariesInGeant4) {
              double newTime = te;
              if (newTime < time) {
                newTime += time;
              }
              fSecondaryParticles->push_back(new GarfieldParticle(
                  "e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
            }

            drift.DriftElectron(xe, ye, ze, te);

            double xe1, ye1, ze1, te1;
            double xe2, ye2, ze2, te2;

            int status;
            //drift.GetEndPoint(xe2, ye2, ze2, te2, status);//Usar com modelo2
            drift.GetElectronEndpoint(0, xe1, ye1, ze1, te1, xe2, ye2, ze2, te2, status); //Usar sem modelo2
             std::cout << "\n\n\t" << FGRN("Distância ao Ânodo:") << "\t" << sqrt(xe2*xe2 + ye2*ye2)/rWire << "\n\n\t";
            if(sqrt(xe2*xe2 + ye2*ye2) < 1.5*rWire){dado = true;}
             if(xe2*xe2 + ye2*ye2 <= 1000*rWire*rWire) std::cout << "\n\n\t\t" << UNDL(BOLD(FRED("ELÉTRONS NO ÂNODO")))<<"\n\n";

            if (0 < xe2 && xe2 < rWire) {
              xe2 += rWire;
            } else if (0 > xe2 && xe2 > -rWire) {
              xe2 += - rWire;
            }
            if (0 < ye2 && ye2 < rWire) {
              ye2 +=  rWire;
            } else if (0 > ye2 && ye2 > -rWire) {
              ye2 += - rWire;
            }

            double e2 = 0.1;
            avalanche.AvalancheElectron(xe2, ye2, ze2, te2, e2, 0, 0, 0);

            int ne = 0, ni = 0;
            avalanche.GetAvalancheSize(ne, ni);
            std::cout << "\t"<<FGRN("Avalanche:")<< "\t"<< ne<<std::endl;
            xFile << ne << "\n";
            yFile << ne << "\t" << te2 << "\n";
            fAvalancheSize += ne;
          }
        }
        std::cout << std::endl<<"\t"<<FGRN("Total:")<< "\t"<< fAvalancheSize<<std::endl;
      }
    }
  }
  fGain = fAvalancheSize / nsum;

  /*fSensor->ConvoluteSignals();
  int nt = 0;
  //if (!fSensor->ComputeThresholdCrossings(-2., "Anodo", nt)) continue;
  if (true) signalView.PlotSignal("Anodo",true,false,false,false,true);*/
  
}

std::vector<GarfieldParticle*>* GarfieldDetector::GetSecondaryParticles() {
  return fSecondaryParticles;
}

void GarfieldDetector::DeleteSecondaryParticles() {
  if (!fSecondaryParticles->empty()) {
    fSecondaryParticles->erase(fSecondaryParticles->begin(),
                               fSecondaryParticles->end());
  }
}