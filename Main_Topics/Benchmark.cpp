/*
Si scriva la parte rilevante ed autoconsistente di codice di una macro di ROOT
rivolto a monitorare con la classe TBenchmark il tempo di CPU rispettivamente
impiegato per fare le due seguenti operazioni, utilizzando anche l’opportuno
metodo per stampare a schermo i tempi di esecuzione:
1. Generare 105 occorrenze generate esplicitamente e singolarmente di una
gaussiana con media µ = 0 e deviazione standard σ = 1, riempiendo un istogramma
h1 che si assume già opportunamente definito.
2. Generare 105 occorrenze di una gaussiana G(0,1) con il metodo FillRandom,
riempiendo un istogramma h2 che si assume già opportunamente definito
*/

#include <TBenchmark.h>
#include <TH1.h>
#include <TRandom.h>

int macro() {

    TH1D *histo = new TH1D("histo", "Histogram", 1000, -5, 5);
    constexpr int n_Gen = 1E5;
    double x = 0;
    gBenchmark->Start("for loop");

    gRandom->SetSeed();
    for (int i = 0; i < n_Gen; i++) {
        x = gRandom->Gaus(0, 1);
        histo->Fill(x);
    }

    gBenchmark->Show("for loop");

    histo->Reset();
    gBenchmark->Start("fillRandom");

    histo->FillRandom("gaus", n_Gen);

    gBenchmark->Show("fillRandom");
    return 0;
}