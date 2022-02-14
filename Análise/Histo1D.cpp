#include "TH1F.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "TF1.h"
#include "TMath.h"

Double_t Polya(Double_t *x, Double_t *par) {
   return par[0]*TMath::Power(x[0]/par[1],par[2]) *TMath::Exp(-(1+par[2])*(x[0]/par[1]));
}

double FindMax(std::string& name){

    std::fstream arq;
    double temp2;
    double xmax;
    arq.open(name, std::ios::in);
    arq >> xmax;
    while(true){
        arq >> temp2;
        if(temp2 > xmax){
            xmax = temp2;
        }
        if(arq.eof()) break;
    }
    arq.close();
    return xmax;
}

int FindN(std::string& name){
    
    std::fstream arq;
    double temp2;
    int i=0;
    arq.open(name, std::ios::in);
    while(true){
        arq >> temp2;
        i++;
        if(arq.eof()) break;
    }
    arq.close();
    return i;
}

void Histo1D(char* nArq, double xmax = 50000, int nDiv = 100, char* unit = "u.a."){

    TH1F *Histo1D = new TH1F("Número de Elétrons", "", nDiv, 0, xmax);
    TCanvas *c1 = new TCanvas();
    std::fstream file;

    std::string nome1 = nArq;
    int n = nome1.size();
    nome1.erase(n-3,3);
    nome1= nome1+"pdf";
    char *nome2 = &nome1[0];

    file.open(nArq, std::ios::in);

    float x;

    while(1){

        file >> x;
        Histo1D->Fill(x);
        if(file.eof()) break;

    }

    file.close();

    std::string nome3 = unit;
    nome3 = "#bf{["+nome3;
    nome3 = nome3+"]}";
    char*nome4 = &nome3[0];

    
    Histo1D->SetXTitle(nome4);
    Histo1D->SetYTitle("");
    Histo1D->GetYaxis()->SetMaxDigits(2);
    Histo1D->GetXaxis()->SetMaxDigits(3);
    Histo1D->SetLineColor(kBlack);
    Histo1D->SetFillColor(kBlack);
    Histo1D->SetFillStyle(3003);
    Histo1D->SetStats(0);
    Histo1D->Draw();

    /*TF1 *ajPolya = new TF1("Polya", Polya, 0, xmax, 3);

    ajPolya->SetParameter(0, 120);
    ajPolya->SetParameter(1, p2);
    ajPolya->SetParameter(2, 0.4);

    Histo1D->Fit("Polya");*/

    c1->Print(nome2);

}

int main(int argc,char** argv){

    std::string arquivo, nomearquivo, maximo, divisao, parametro;

    switch (argc){
        case 2:
            nomearquivo = argv[1];
            Histo1D(argv[1], TMath::Ceil(FindMax(nomearquivo)/500)*500, TMath::CeilNint(TMath::Sqrt(FindN(nomearquivo))));
            break;
        case 3:
            nomearquivo = argv[1];
            maximo = argv[2];
            Histo1D(argv[1], std::stod(maximo), TMath::CeilNint(TMath::Sqrt(FindN(nomearquivo))));
            break;
        case 4:
            maximo = argv[2];
            divisao = argv[3];
            Histo1D(argv[1], std::stod(maximo), std::stoi(divisao));
            break;
        case 5:
            maximo = argv[2];
            divisao = argv[3];
            Histo1D(argv[1], std::stod(maximo), std::stoi(divisao), argv[4]);
            break;
        default:
            break;
    }
    return 0;
}