#include "TFile.h"
#include "TTreeReader.h"
#include <fstream>

TFile file("filename.root");
TTreeReader reader("treename", &file);
TTreeReaderValue<float> Eabs(reader, "Eabs");
TTreeReaderValue<float> Labs(reader, "Labs");
TTreeReaderValue<float> EGas(reader, "EGas"); 
TTreeReaderValue<float> TAvalanche(reader, "TAvalanche");
TTreeReaderValue<float> Ganho(reader, "Ganho"); 

void tree(){
    std::ofstream eFile("Dados.txt",std::ofstream::out);
    while (reader.Next()) {
        eFile << *Eabs << "\t" << *Labs << "\t" << *EGas << "\t" << *TAvalanche <<"\t"<< *Ganho<<"\n";
    }
}