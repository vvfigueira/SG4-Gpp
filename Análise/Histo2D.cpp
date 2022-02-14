#include "TH2F.h"
#include "TCanvas.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "TF1.h"

void Histo2D(char* nArq, int nDivx = 100, int nDivy = 5, double xmax = 50000, double ymax = 13000){

    TH2F *Histo2D = new TH2F("Energia vs Potencial","",nDivx,0,xmax,nDivy,0,ymax);
    TCanvas *c1 = new TCanvas();
    std::fstream file;

    std::string nome1 = nArq;
    nome1= nome1+".pdf";
    char *nome2 = &nome1[0];

    file.open(nArq, std::ios::in);

    float x, y;

    while(1){

        file >> x >> y;
        Histo2D->Fill(x,y);
        if(file.eof()) break;

    }
    Histo2D->SetXTitle("#bf{[eV]}");
    Histo2D->SetYTitle("#bf{[u.a.]}");
    Histo2D->GetYaxis()->SetMaxDigits(3);
    Histo2D->GetXaxis()->SetMaxDigits(3);
    Histo2D->SetMarkerColor(1);
    Histo2D->SetLineColor(1);
    Histo2D->SetStats(0);
    Histo2D->Draw("COLZ");
    /*TF1 *ajt = new TF1("ajst", "[0]*(x/([1]))^[2] *exp(-(1+[2])*(x/([1])))",0,50000);

    ajt->SetParameter(0, 120);
    ajt->SetParameter(1, 1554);
    ajt->SetParameter(2, 0.4);
    Histo2D->Fit("ajst");*/

    c1->Print(nome2);

}

int main(int argc,char** argv){

    switch (argc){
        case 2:
            Histo2D(argv[1]);
            break;
        case 3:
            Histo2D(argv[1], std::stoi(argv[2]));
            break;
        case 4:
            Histo2D(argv[1], std::stoi(argv[2]), std::stoi(argv[3]));
            break;
        case 5:
            Histo2D(argv[1], std::stoi(argv[2]), std::stoi(argv[3]), std::stod(argv[4]));
            break;
        case 6:
            Histo2D(argv[1], std::stoi(argv[2]), std::stoi(argv[3]), std::stod(argv[4]), std::stod(argv[5]));
        default:
            break;
    }
    return 0;
}