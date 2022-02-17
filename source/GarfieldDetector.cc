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

// Inicialização de variável

GarfieldDetector* GarfieldDetector::fGarfieldDetector = 0;

// Método para criar o detector do Garfield++

GarfieldDetector* GarfieldDetector::GetInstance() {
    if (!fGarfieldDetector) {
        fGarfieldDetector = new GarfieldDetector();
    }
    return fGarfieldDetector;
}

// Método para deletar o detector do Garfield++

void GarfieldDetector::Dispose() {
    delete fGarfieldDetector;
    fGarfieldDetector = 0;
}

// Construtor inicializando variáveis

GarfieldDetector::GarfieldDetector() {
    fSecondaryParticles = new std::vector<GarfieldParticle*>();
    fMediumMagboltz = 0;
    fSensor = 0;
    fComponentAnalyticField = 0;
    fTrackHeed = 0;
    fAvalMC = 0;
    fAvalmc = 0;
    fDrift = 0;
    createSecondariesInGeant4 = true;
    fIonizationModel = "PAIPhot";
    xFile.open("AvaPEle.txt", std::ofstream::app);
    yFile.open("AvaPTemp.txt", std::ofstream::app);
    zFile.open("AvaTot.txt", std::ofstream::app);
    aFile.open("EleEner.txt", std::ofstream::app);
    bFile.open("ElePos.txt", std::ofstream::app);
    cFile.open("EEnerPos.txt", std::ofstream::app);
}

// Aniquilador

GarfieldDetector::~GarfieldDetector() {
    DeleteSecondaryParticles();
    delete fSecondaryParticles;
    delete fMediumMagboltz;
    delete fSensor;
    delete fComponentAnalyticField;
    delete fTrackHeed;
    delete fAvalMC;
    delete fAvalmc;
    delete fDrift;
    xFile.close();
    yFile.close();
    zFile.close();
    aFile.close();
    bFile.close();
    cFile.close();

    std::cout << "Aniquilando GarfieldDetector" << std::endl;
}

// Método para retornar o Modelo de Ionização

std::string GarfieldDetector::GetIonizationModel(){return fIonizationModel;}

// Método para ajuste do Modelo de Ionização

void GarfieldDetector::SetIonizationModel(std::string model, bool useDefaults) {
    if (model != "PAIPhot" && model != "PAI" && model != "Heed") {
        std::cout << "Modelo de ionização desconhecido " << model << std::endl;
        std::cout << "Usando PAIPhot como modelo padrão!" << std::endl;
        model = "PAIPhot";
    }
    fIonizationModel = model;

    if (fIonizationModel == "PAIPhot" || fIonizationModel == "PAI") {
        if (useDefaults == true) {

            // Partículas e energias para as quais G4FastSimulationModel com
            // Garfield++ é válido

            this->AddParticleName("e-", 1e-6, 1e-3, "garfield");
            this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");            

            // Partículas e energias para as quais os modelos PAI ou PAIPhot 
            // são válidos

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

            // Partículas e energias para as quais G4FastSimulationModel com
            // Garfield++ é válido

            this->AddParticleName("gamma", 1e-6, 1e+8, "garfield");
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

// Adição de partículas

void GarfieldDetector::AddParticleName(const std::string particleName,
    double ekin_min_MeV, double ekin_max_MeV, std::string program) {
    if (ekin_min_MeV >= ekin_max_MeV) {
        std::cout << "Ekin_min=" << ekin_min_MeV
            << " keV é maior que Ekin_max=" << ekin_max_MeV << " keV"
            << std::endl;
        return;
    }

    if (program == "garfield") {
        std::cout << "Modelo do Garfield (Heed) é aplicável para G4Particle "
            << particleName << " entre " << ekin_min_MeV << " MeV e "
            << ekin_max_MeV << " MeV" << std::endl;

        fMapParticlesEnergyGarfield.insert(std::make_pair(
            particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
    } else {
        std::cout << fIonizationModel << " é aplicável para G4Particle "
            << particleName << " entre " << ekin_min_MeV << " MeV e "
            << ekin_max_MeV << " MeV" << std::endl;
        fMapParticlesEnergyGeant4.insert(std::make_pair(
            particleName, std::make_pair(ekin_min_MeV, ekin_max_MeV)));
    }
}

// Procura se a partícula está definida no Garfield++ ou no Geant4

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

// Verifica se a energia informada está dentro dos limites estabelecidos para a partícula

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

// Retorna a energia mínima definida para a partícula

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

// Retorna a energia máxima definida para a partícula

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

// Construção dos elementos do Detector do Garfield++

void GarfieldDetector::InitializePhysics() {
    
    // Definição da mistura gasosa

    fMediumMagboltz = new Garfield::MediumMagboltz();
    fMediumMagboltz->SetComposition("ar", 70., "co2", 30.);
    fMediumMagboltz->SetTemperature(Dim::temperaturagas/kelvin);
    fMediumMagboltz->SetPressure(Dim::pressaogas/torr);
    fMediumMagboltz->Initialise(true);

    // Definição do coeficiente de Penning

    const double rPenning = 0.57;
    const double lambdaPenning = 0.;
    fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");

    // Indicando o arquivo .gas

    fMediumMagboltz->LoadGasFile("ar_70_co2_30_1000mbar.gas");

    // Inicialização dos componentes com potencial

    fComponentAnalyticField = new Garfield::ComponentAnalyticField();
    fComponentAnalyticField->SetMedium(fMediumMagboltz);

    // Definição do Ânodo de Ouro

    fComponentAnalyticField->AddWire(0., 0., 2 * Dim::raiofioouro/cm, 
        Dim::potencialanodo/volt, "Anodo");

    // Definição do Cátodo Interno

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

    // Definição do Cátodo Externo

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

    // Definição do Cátodo de Alumínio

    fComponentAnalyticField->AddTube((Dim::raioextal+Dim::raiointal)/(2*cm), 
        Dim::potencialal, 0, "Aluminio");

    // Cálculo do campo

    fComponentAnalyticField->AddReadout("Anodo");

    // Definição do Sensor

    fSensor = new Garfield::Sensor();
    fSensor->AddComponent(fComponentAnalyticField);
    fSensor->SetArea();

    // Definição do Método de transporte TrackHeed

    fTrackHeed = new Garfield::TrackHeed();
    fTrackHeed->SetSensor(fSensor);
    fTrackHeed->EnableDeltaElectronTransport();
    fTrackHeed->EnableElectricField();
    fTrackHeed->EnablePhotonReabsorption();

    fAvalMC = new Garfield::AvalancheMC();
    fAvalMC->SetSensor(fSensor);
    fAvalMC->SetDistanceSteps(1.e-4);

    fAvalmc = new Garfield::AvalancheMicroscopic();
    fAvalmc->SetSensor(fSensor);
    fAvalmc->DisableAvalancheSizeLimit();
}

void GarfieldDetector::DoIt(std::string particleName, double ekin_MeV,
    double time, double x_cm, double y_cm, double z_cm,
    double dx, double dy, double dz) {

    fEnergyDeposit = 0;
    DeleteSecondaryParticles();

    // Raio do Ânodo

    const double rWire = Dim::raiofioouro/cm;

    // Raio do Cátodo de Alumínio
    
    const double rTube = (Dim::raioextal+Dim::raiointal)/(2*cm);

    // Comprimento do detector
    
    const double lTube = Dim::comprimento/cm;

    double eKin_eV = ekin_MeV * 1e+6;

    double xc = 0., yc = 0., zc = 0., tc = 0.;

    // Número de elétrons produzidos na ionização
    
    int nc = 0;

    // Perda de energia na ionização

    double ec = 0.;

    double extra = 0.;

    fEnergyDeposit = 0;

    if (fIonizationModel != "Heed" || particleName == "gamma") {
        if (particleName == "gamma") {

            // Inicialização de um fóton no gás

            fTrackHeed->TransportPhoton(x_cm, y_cm, z_cm, time, eKin_eV, dx, dy, dz, nc);
            std::cout << std::endl<<std::endl<< "\t"<<FGRN("Elétrons Gerados:")  
                << "\t" << nc << std::endl;
        } else {

            // Inicialização de um elétron no gás

            fTrackHeed->TransportDeltaElectron(x_cm, y_cm, z_cm, time, eKin_eV, dx,
                dy, dz, nc);
            fEnergyDeposit = eKin_eV;
            std::cout << std::endl<<std::endl<< "\t"<<FGRN("Ganho:")  << "\t" << nc 
                << std::endl;
        }
        for (int cl = 0; cl < nc; cl++) {
            double xe, ye, ze, te, ee, dxe, dye, dze;

            // Referência ao elétron nº 'cl' gerado pela ionização

            fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
            if (ze < lTube && ze > -lTube && sqrt(xe * xe + ye * ye) < rTube) {
                nsum++;
                if (particleName == "gamma") {
                    fEnergyDeposit += fTrackHeed->GetW();
                }
                if (createSecondariesInGeant4) {
                    double newTime = te;
                    if (newTime < time) {
                        newTime += time;
                    }
                    fSecondaryParticles->push_back(new GarfieldParticle(
                        "e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
                }

                // Drift do elétron

                fAvalMC->DriftElectron(xe, ye, ze, te);

                double xe1, ye1, ze1, te1, ee1;
                double xe2, ye2, ze2, te2, ee2;

                int status;

                // Referência ao ponto final de drift do elétron
                
                fAvalMC->GetElectronEndpoint(0, xe1, ye1, ze1, te1, xe2, ye2, 
                    ze2, te2, status );
                std::cout << "\n\n\t" << FGRN("Distância ao Ânodo:") << "\t" 
                    << sqrt(xe2*xe2 + ye2*ye2)/rWire << "\n\n\t";
                if(xe2*xe2 + ye2*ye2 <= 1000*rWire*rWire) std::cout << "\n\n\t\t" 
                    << UNDL(BOLD(FRED("ELÉTRONS NO ÂNODO")))<<"\n\n";                        

                // Truque caso o elétron penetre no Ânodo

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

                // Avalanche do elétron

                fAvalmc->AvalancheElectron(xe2, ye2, ze2, te2, e2, 0, 0, 0);

                int ne = 0, ni = 0;
                fAvalmc->GetAvalancheSize(ne, ni);
                int Ne = 0;
                Ne = fAvalmc->GetNumberOfElectronEndpoints();

                // Tomada de dados

                for(int k= 0;k<Ne;k++){
                    fAvalmc->GetElectronEndpoint(k,xe1,ye1,ze1,te1,ee1,xe2,ye2,
                        ze2,te2,ee2,status);
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

        // Transporte e ionização pelo Heed

        fTrackHeed->SetParticle(particleName);
        fTrackHeed->SetKineticEnergy(eKin_eV);
        fTrackHeed->NewTrack(x_cm, y_cm, z_cm, time, dx, dy, dz);

        // Refrência as ionizações

        while (fTrackHeed->GetCluster(xc, yc, zc, tc, nc, ec, extra)) {
            if(particleName == "e-" || particleName == "electron"){
                std::cout << std::endl<<std::endl<< "\t"<<FGRN("Ganho:")  
                    << "\t" << nc << std::endl;
            }else{
                std::cout << std::endl<<std::endl<< "\t"<<FGRN("Elétrons Gerados:")  
                    << "\t" << nc << std::endl;
            }      
            if (zc < lTube && zc > -lTube && sqrt(xc * xc + yc * yc) < rTube) {
                nsum += nc;
                fEnergyDeposit += ec;
                for (int cl = 0; cl < nc; cl++) {
                    double xe, ye, ze, te;
                    double ee, dxe, dye, dze;

                    // Referência ao elétron nº 'cl' das ionizações

                    fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
                    if (ze < lTube && ze > -lTube && sqrt(xe * xe + ye * ye) < rTube) {
                        if (createSecondariesInGeant4) {
                            double newTime = te;
                            if (newTime < time) {
                                newTime += time;
                            }
                            fSecondaryParticles->push_back(new GarfieldParticle(
                                "e-", ee, newTime, xe, ye, ze, dxe, dye, dze));
                        }

                        // Drift do elétron

                        fAvalMC->DriftElectron(xe, ye, ze, te);

                        double xe1, ye1, ze1, te1;
                        double xe2, ye2, ze2, te2;

                        int status;
                        
                        // Referência ao ponto final do elétron

                        fAvalMC->GetElectronEndpoint(0, xe1, ye1, ze1, te1, xe2, ye2, ze2, 
                            te2, status);
                        std::cout << "\n\n\t" << FGRN("Distância ao Ânodo:") << "\t" 
                            << sqrt(xe2*xe2 + ye2*ye2)/rWire << "\n\n\t";
                        if(xe2*xe2 + ye2*ye2 <= 1000*rWire*rWire) std::cout << "\n\n\t\t" 
                            << UNDL(BOLD(FRED("ELÉTRONS NO ÂNODO")))<<"\n\n";

                        // Truque caso o elétron penetre no ânodo

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

                        // Avalanche do elétron

                        double e2 = 0.1;
                        fAvalmc->AvalancheElectron(xe2, ye2, ze2, te2, e2, 0, 0, 0);

                        int ne = 0, ni = 0;
                        fAvalmc->GetAvalancheSize(ne, ni);
                        std::cout << "\t"<<FGRN("Avalanche:")<< "\t"<< ne<<std::endl;

                        // Tomada de dados

                        xFile << ne << "\n";
                        yFile << ne << "\t" << te2 << "\n";
                        fAvalancheSize += ne;
                    }
                }
                std::cout << std::endl<<"\t"<<FGRN("Total:")<< "\t"<< fAvalancheSize
                    <<std::endl;
            }
        }
    }
    fGain = fAvalancheSize / nsum;  
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