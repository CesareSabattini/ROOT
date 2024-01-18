#include <TCanvas.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom.h>

double efficiency(double x) {
    double val = 0.1 * x * exp(-x);
    return val;
}

int macro1() {
    TH1D *histo1 = new TH1D("histo1", "Gauss", 1000, 0, 10);
    histo1->Sumw2();
    TH1D *histo2 = new TH1D("histo2", "Efficiency", 1000, 0, 10);
    histo2->Sumw2();

    constexpr int n_Gen = 1E7;
    double x = 0;
    double probability = 0;
    gRandom->SetSeed();

    for (int i = 0; i < n_Gen; i++) {
        x = gRandom->Gaus(5, 1);
        histo1->Fill(x);
        probability = gRandom->Rndm();
        if (probability < efficiency(x)) {
            histo2->Fill(x);
        }
    }

    TH1D *histoEfficiency =
        new TH1D("histoEfficiency", "Efficiency", 1000, 0, 10);
    histoEfficiency->Divide(histo1, histo2, 1, 1, "B");

    TCanvas *canvas = new TCanvas("canvas", "Graphs", 700, 700);
    canvas->Divide(2, 2);
    canvas->cd(1);
    histo1->Draw();
    canvas->cd(2);
    histo2->Draw();
    canvas->cd(3);
    histoEfficiency->Draw("E");

    return 0;
}

double fitFunction(double *x, double *par) {
    double xx = x[0];
    double val =
        par[0] * exp(-(xx - par[1]) * (xx - par[1]) / (2 * par[2] * par[2])) +
        par[3] * exp(par[4] * xx);
    return val;
}

int macro2() {
    TH1D *histo1 = new TH1D("histo1", "Gauss", 500, 0, 5);
    histo1->Sumw2();
    TH1D *histo2 = new TH1D("histo2", "Exponential", 500, 0, 5);
    histo2->Sumw2();

    constexpr int n_Gen_1 = 1E6;
    constexpr int n_Gen_2 = 1E5;
    double x = 0;

    gRandom->SetSeed();

    for (int i = 0; i < n_Gen_1; i++) {
        x = gRandom->Gaus(2, 0.5);
        histo1->Fill(x);
    }

    for (int i = 0; i < n_Gen_2; i++) {
        x = gRandom->Exp(1);
        histo2->Fill(x);
    }

    TH1D *histoSum = new TH1D("histoSum", "Sum", 500, 0, 5);
    histoSum->Add(histo1, histo2, 1, 1);

    TF1 *fit = new TF1("fit", fitFunction, 0, 5, 5);
    fit->SetParameter(0, 8000);
    fit->SetParameter(1, 2);
    fit->SetParameter(2, 0.6);
    fit->SetParameter(3, 1500);
    fit->SetParameter(4, 1);
    histoSum->Fit(fit, "Q");

    TCanvas *canvas1 = new TCanvas("canvas1", "Histos", 500, 500);
    canvas1->Divide(2, 2);
    canvas1->cd(1);
    histo1->Draw();
    canvas1->cd(2);
    histo2->Draw();
    canvas1->cd(3);
    histoSum->Draw();

    std::cout << "DATA:" << std::endl << std::endl;

    for (int i = 0; i < 6; i++) {
        std::cout << "par[" << i << "]= " << fit->GetParameter(i) << " +/- "
                  << fit->GetParError(i) << std::endl;
    }

    std::cout << "Reduced Chi Square= " << fit->GetChisquare() / fit->GetNDF()
              << std::endl;

    return 0;
}

int macro3() {
    TH1D *histo = new TH1D("histo", "Histogram", 100, 0, 10);
    TF1 *fit = new TF1("fit", "sin(x)+x^2", 0, 10);
    constexpr int n_Gen = 1E5;
    histo->FillRandom("fit", n_Gen);
    histo->Draw();
    return 0;
}