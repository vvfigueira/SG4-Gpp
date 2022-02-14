#include "TCanvas.h"
#include "TGraphErrors.h"
#include <fstream>
#include <iostream>
#include "TFile.h"
#include "TF1.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TRatioPlot.h"
#include "TMath.h"
#include "TMathText.h"
#include "TPad.h"

Double_t xmin = 36000, xmax = 38000;
Double_t ymin = 3000, ymax = 40000;

int npar = 3;

Double_t Ajt(Double_t *x, Double_t *par) {
    return par[0]*(1-par[1]*TMath::Sin(TMath::DegToRad()*2*x[0])+par[2]*
        TMath::Cos(TMath::DegToRad()*2*x[0]));
}

Double_t Polya(Double_t *x, Double_t *par) {
   return par[0]*TMath::Power(x[0]/par[1],par[2]) *TMath::Exp(-(1+par[2])*(x[0]/par[1]));
}

void gp(){
    
    TLatex *latex = new TLatex();

    TCanvas *c1 = new TCanvas();
    auto pad1 = new TPad("graph","",0.,0.3,1,0.95);

    auto pad2 = new TPad("rezid","",0.,0.,1,0.32);

    pad1->SetFillStyle(4000);
    pad2->SetFillStyle(4000);
    pad1->SetMargin(0.08,0.08,0.01,0);
    pad2->SetMargin(0.08,0.08,0.21,0.01);
    pad1->SetGridx();
    pad1->SetGridy();
    pad2->SetGridx();
    pad2->SetGridy();
    pad2->Draw();
    pad1->Draw();
    

    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();

    TGraphErrors *gr = new TGraphErrors();

    gr->GetXaxis()->SetTitle("#bf{[t]}");
    gr->GetYaxis()->SetTitle("#bf{[N]}");
    gr->GetXaxis()->SetLabelFont(132);
    gr->GetXaxis()->SetTitleFont(132);
    gr->GetYaxis()->SetLabelFont(132);
    gr->GetYaxis()->SetTitleFont(132);
    gr->GetYaxis()->SetLabelSize(0.05);
    gr->GetYaxis()->SetTitleSize(0.05);
    gr->GetXaxis()->SetLabelSize(0.05);
    gr->GetXaxis()->SetTitleSize(0.05);
    gr->GetYaxis()->SetTitleOffset(0.5);
    //gr->SetMarkerStyle(kFullCircle);
    //gr->SetMarkerSize(0.7);

    std::fstream file;
    file.open("Grafico.txt", std::ios::in);

    double x1[2],ex = 1, ey = 0.05;

    int n = 0;

    while(1){
        file >> x1[0] >> x1[1]; //>> x1[2] >> x1[3] >> x1[4];
    
        n = gr->GetN();

        gr->SetPoint(n, x1[1], x1[0]);
        gr->SetPointError(n, ex, ey);

        if(file.eof()) break;
    
    }

    file.close();

    gr->GetXaxis()->SetLimits(xmin, xmax);
    //gr->GetYaxis()->SetRangeUser(6, 102);
    pad1->cd();
    gr->Draw("AP");

    TF1 *mal = new TF1("mal", Polya, xmin, xmax, npar);
    mal->SetLineColor(kRed);
    mal->SetParameter(0, 120);
    mal->SetParameter(1, 100);
    mal->SetParameter(2, 0.4);
    //mal->SetParameter(3, 0);

    gr->Fit("mal","","",xmin,xmax);

    Double_t par[npar];

    mal->GetParameters(par);
    
    //std::cout << gr->Chisquare(mal) << "\n";

    auto *red = new TGraphErrors();

    int m = 0;

    std::fstream file2;
    file2.open("Grafico.txt", std::ios::in);

    Double_t x2[2];

    while(1){
        
        file2 >> x2[0] >> x2[1]; //>> x2[2] >> x2[3] >> x2[4];
    
        m = red->GetN();

        red->SetPoint(m, x2[1], -(Ajt(x2,par) - x2[0]));
        red->SetPointError(m, ex, ey);

        if(file2.eof()) break;

    }

    file2.close();

    red->GetXaxis()->SetLimits(xmin, xmax);
    /*auto c2 = new TCanvas();

    c2->SetTickx();
    c2->SetTicky();
    c2->SetGridx();
    c2->SetGridy();*/

    red->GetXaxis()->SetTitle("#bf{[t]}");
    //red->GetYaxis()->SetTitle("#bf{[N]}");
    red->GetXaxis()->SetLabelFont(132);
    red->GetXaxis()->SetTitleFont(132);
    red->GetYaxis()->SetLabelFont(132);
    red->GetYaxis()->SetTitleFont(132);
    red->GetYaxis()->SetTitleOffset(1.3);
    red->GetYaxis()->SetLabelSize(0.1);
    red->GetYaxis()->SetTitleSize(0.1);
    red->GetXaxis()->SetLabelSize(0.1);
    red->GetXaxis()->SetTitleSize(0.1);
    //red->GetYaxis()->SetRangeUser(-0.05, 0.058);

    auto fnc = new TF1("fnc",Ajt,xmin,xmax,npar);

    fnc->SetParameters(par);
    pad2->cd();
    red->Draw("AP");

    //fnc->Draw("SAME");

    /*TLegend *leg = new TLegend(0.68, 0.705, 0.82, 0.805);
    leg->SetBorderSize(0);
    leg->AddEntry(gr, "Dados", "p");
    leg->AddEntry(mal, "Ajuste", "l");
    leg->Draw();*/

}