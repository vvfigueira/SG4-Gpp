#include "TH1F.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "TF1.h"
#include "TMath.h"

void Eli(char* nArq,double cut){
    std::fstream file;
    file.open(nArq,std::ios::in);

    float x;

    std::string nome1 = nArq;
    int n = nome1.size();
    nome1.erase(n-4,4);
    nome1= nome1+"_mod.txt";
    char *nome2 = &nome1[0];

    std::ofstream zFile (nome2, std::ofstream::app);

    while(1){
        file >> x;
        if(x > cut)zFile << x<<"\n";
        if(file.eof()) break;

    }

}

int main(int argc,char** argv){

    std::string arquivo, nomearquivo, maximo, divisao, parametro;

    switch (argc){
        case 3:
            Eli(argv[1],std::stod(argv[2]));
            break;
        default:
            std::cout << "\nUso: ./Eli [NomeArquivo] [Valor de corte]\n";
            break;
    }
    return 0;
}