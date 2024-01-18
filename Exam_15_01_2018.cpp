#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom.h>
#include <iostream>

double efficiency(double x) { return x / 5; }

double fit1(double *x, double *par) {
    double xx = x[0];
    double val = xx / par[0];
    return val;
}

int macro1() {
    TH1D *histo1 = new TH1D("histo1", "Histo 1", 1000, 0, 5);
    histo1->Sumw2();
    TH1D *histo2 = new TH1D("histo2", "Histo 2", 1000, 0, 5);
    histo2->Sumw2();

    const int n_Gen = 1E7;
    double x = 0;
    gRandom->SetSeed();

    for (int i = 0; i < n_Gen; i++) {
        x = gRandom->Exp(1);
        histo1->Fill(x);
        const double probability = gRandom->Rndm();
        if (probability < efficiency(x)) {
            histo2->Fill(x);
        }
    }

    TH1D *histoDivision = new TH1D("histoDivision", "Division", 1000, 0, 5);
    histoDivision->Divide(histo1, histo2, 1, 1);

    TCanvas *canvas = new TCanvas("canvas", "Canvas", 500, 500);
    histoDivision->Draw();

    return 0;
}

double fit2(double *x, double *par) {
    double xx = x[0];
    double val =
        par[0] * exp(-(xx - par[1]) * (xx - par[1]) / 2 * par[2] * par[2]) +
        par[3];
    return val;
}

int macro2() {
    TH1D *histo1 = new TH1D("histo1", "Gauss", 500, 0, 5);
    histo1->Sumw2();
    TH1D *histo2 = new TH1D("histo2", "Uniform", 500, 0, 5);
    histo2->Sumw2();

    constexpr int n_Gen_1 = 1E6;
    constexpr int n_Gen_2 = 1E4;
    gRandom->SetSeed();
    double x = 0;
    for (int i = 0; i < n_Gen_1; i++) {
        x = gRandom->Gaus(2.5, 0.25);
        histo1->Fill(x);
    }

    for (int i = 0; i < n_Gen_1; i++) {
        x = gRandom->Uniform(0, 5);
        histo2->Fill(x);
    }

    TH1D *histoSum = new TH1D("histoSum", "Sum", 500, 0, 5);
    histoSum->Add(histo1, histo2, 1, 1);

    TF1 *fitFunction = new TF1("fitFunction", fit2, 0, 5, 4);
    fitFunction->SetParameters(1, 2.5, 0.25, 0.2);
    histoSum->Fit(fitFunction);

    std::cout << "DATA:" << std::endl << std::endl;

    for (int i = 0; i < 5; i++) {
        std::cout << 'par[' << i << "]= " << fitFunction->GetParameter(i)
                  << "+/-" << fitFunction->GetParError(i) << std::endl;
    }
    std::cout << "Reduced Chi Square= "
              << fitFunction->GetChisquare() / fitFunction->GetNDF()
              << std::endl;
    return 0;
}

int macro3() {
    TH1D *histo = new TH1D("histo", "Histogram", 100, 0, 10);
    const int n_Gen = 1E7;
    gRandom->SetSeed();
    TF1 *fitFunction = new TF1("fitFunction", "sqrt(x)+x^2", 0, 10);
    histo->FillRandom("fitFunction", n_Gen);
    histo->Draw();
    return 0;
}