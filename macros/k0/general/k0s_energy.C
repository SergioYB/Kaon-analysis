#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"

//Computing total POT in all events
double get_total_POT(TTree* subrun_tree){
    double n_POT = 0;
    set_branch_subtree(subrun_tree);
    int n_subruns = subrun_tree->GetEntries();

    for (int i = 0; i<= n_subruns; i++){
        subrun_tree->GetEntry(i);
        n_POT = n_POT + POT;
    }

    return n_POT;
}

int findPos(const std::vector<int>* v, int a) {
    // Usar std::find para buscar el elemento en el puntero al vector
    auto it = std::find(v->begin(), v->end(), a);

    // Verificar si el elemento fue encontrado
    if (it != v->end()) {
        // Calcular y devolver la posición del elemento
        return std::distance(v->begin(), it);
    } else {
        // Si no se encontró, devolver -1
        return -1;
    }
}



void k0s_energy(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event



    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h1 = new TH1F("h1", "", 15, 0.2, 2.2); //x-> ks energy

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();


        //Loop over g4 particles to find ks from primary k0
        for(int i_p = 0; i_p < size_g4; i_p++){
            if (g4_part_PDGcode->at(i_p) == 310){
                //Its mother is primary k0
                if (g4_part_process->at(findPos(g4_part_trackID, g4_part_mother->at(i_p))) == "primary"){
                    h1->Fill(g4_part_E0->at(i_p));
                }
            }
        }
    
    }

    //Draw histogram
    h1->SetStats(0);

    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);

    h1->GetXaxis()->SetTitle("K^{0}_{S} E0 [GeV]");
    h1->GetYaxis()->SetTitle("Events");

    //h1->Scale(1e21/(3 * n_POT));

    //Color settings

    h1->SetLineColor(kGreen);
    h1->SetLineWidth(3);

    h1->SetFillColor(kGreen-7);
    h1->SetFillStyle(1001); 

    h1->Draw("HIST");

}