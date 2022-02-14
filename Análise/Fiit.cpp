#include "TH1.h"
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TRatioPlot.h"
#include <fstream>

Double_t Polya(Double_t *x, Double_t *par) {
   return par[0]*TMath::Power(x[0]/par[1],par[2]) *TMath::Exp(-(1+par[2])*(x[0]/par[1]));
}

Double_t Polya2(Double_t *x, Double_t *par) {
    return TMath::Max(par[0]*TMath::Power((x[0]-par[3])/par[1],par[2]) *TMath::Exp(-(1+par[2])*((x[0]-par[3])/par[1])),1.e-10);
}

Double_t Noise(Double_t *x,Double_t *par){
    return Polya(x ,par) + Polya2(x,&par[3]);
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


void Fiit(char* nArq, double xmax = 50000, int nDiv = 100, double p2 = 1500) {

    TCanvas* c1 = new TCanvas();

    std::fstream file;

    std::string nome1 = nArq;
    int n = nome1.size();
    nome1.erase(n-3,3);
    nome1= nome1+"pdf";
    char *nome2 = &nome1[0];

    file.open(nArq, std::ios::in);

    TH1F *hist = new TH1F("", "", nDiv, 0, xmax);

    float x;

    while(1){

        file >> x;
        hist->Fill(x);
        if(file.eof()) break;

    }

    file.close();

    hist->SetXTitle("#bf{[eV]}");
    hist->SetYTitle("Contagem");
    hist->GetYaxis()->SetMaxDigits(2);
    hist->GetXaxis()->SetMaxDigits(3);
    hist->SetLineColor(kBlack);
    hist->SetFillColor(kBlack);
    hist->SetFillStyle(3003);
    hist->SetStats(0);

    hist->Draw();
    
    TF1 *fitFcn = new TF1("fitFcn",Noise,0,xmax, 7);
    
    fitFcn->SetNpx(500);
    fitFcn->SetLineWidth(4);
    fitFcn->SetLineColor(kBlack); 
    fitFcn->SetParameter(2, 0.4);
    fitFcn->SetParameter(3, 12);
    fitFcn->SetParameter(4, hist->GetMean()/20.);
    fitFcn->SetParameter(5, 0.04);
    fitFcn->SetParameter(6, p2);
    fitFcn->SetParLimits(4,0,10000000);
    fitFcn->SetParLimits(3,0,10000000);
    fitFcn->SetParLimits(2,0,10000000);
    fitFcn->SetParLimits(6,0,10000000);

    hist->Fit("fitFcn");
    hist->Draw("");

    TF1 *P = new TF1("P",Polya,0,xmax,3);
    P->SetLineColor(kRed);
    TF1 *teste2 = new TF1("teste2",Polya2,0,xmax,4);
    teste2->SetLineColor(kBlue);
    Double_t par[7];

    fitFcn->GetParameters(par);
    P->SetParameters(par);
    P->Draw("same");
    teste2->SetParameters(&par[3]);
    teste2->Draw("same");

    /*TLegend *legend=new TLegend(0.6,0.65,0.88,0.85);
    legend->SetTextFont(72);
    legend->SetTextSize(0.04);
    legend->AddEntry(hist,"Dados","lpf");
    legend->AddEntry(P,"Ruido de fundo","l");
    legend->AddEntry(teste2,"Sinal","l");
    legend->AddEntry(fitFcn,"Ajuste Global","l");
    legend->Draw();*/

    c1->Print(nome2);
}

int main(int argc,char** argv){

    std::string arquivo, nomearquivo, maximo, divisao, parametro;

    switch (argc){
        case 2:
            nomearquivo = argv[1];
            Fiit(argv[1], TMath::Ceil(FindMax(nomearquivo)/500)*500, TMath::Ceil(TMath::Sqrt(FindN(nomearquivo))));
            break;
        case 3:
            nomearquivo = argv[1];
            maximo = argv[2];
            Fiit(argv[1], std::stod(maximo), TMath::CeilNint(TMath::Sqrt(FindN(nomearquivo))));
            break;
        case 4:
            maximo = argv[2];
            divisao = argv[3];
            Fiit(argv[1], std::stod(maximo), std::stoi(divisao));
            break;
        case 5:
            maximo = argv[2];
            divisao = argv[3];
            parametro = argv[4];
            Fiit(argv[1], std::stod(maximo), std::stoi(divisao), std::stod(parametro));
            break;
        default:
            break;
    }
    return 0;
}