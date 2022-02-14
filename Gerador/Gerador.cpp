#include <fstream>

int main(){
    
    std::ofstream ofs;
    ofs.open("Macro.mac", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    std::ofstream eFile ("Macro.mac" ,std::ofstream::app);

    double pressInit = 133.322;
    double passoP = 133.322/5;
    int repetP = 65;
    bool Pconst = true;

    double tempInit = 0;
    double passoT = 50;
    int repetT = 12;
    bool Tconst = true;

    double campoInit = 12500;
    double passoC = 500;
    int repetC = 25;
    bool Cconst = true;

    double enerInit = 200;
    double passoE = campoInit;
    int repetE = 9;
    bool Econst = true;

    int nPart = 1000000;

    eFile << "/control/verbose 0\n"
          << "/process/em/verbose 0\n"
          << "/process/verbose 0\n"
          << "/process/had/verbose 0\n"
          << "/run/verbose 1\n" 
          << "/run/initialize\n\n"
          << "/gun/particle gamma\n"
          << "/gun/position 0 0.05 0 m\n"
          << "/gun/direction 1 0 0\n"
          << "/gun/energy 200 eV\n\n";

    eFile << "/Detector/DefGasPressao " << pressInit<< " Pa\n"; 
    eFile << "/Detector/DefCampoInt " << campoInit << " V\n\n";
    for(int i = 0; i<= repetT; i++){
        eFile << "/Detector/DefGasTemp " << tempInit +passoT*i<< " K\n"
        
              << "/run/reinitializeGeometry\n"
              << "/run/beamOn " << nPart <<"\n\n";
    }
    eFile.close();

    return 0;
}