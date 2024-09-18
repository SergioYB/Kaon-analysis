#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>

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

std::vector<int> find_events_pipm(TTree* event_tree){

    int n_events = event_tree->GetEntries();
    float size_g4;

    bool pipm_found;

    std::vector<int> events_pipm;

    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();

        //Initialice finding booleans
        pipm_found = false;

        for(int i_p = 0; i_p < size_g4; i_p++){
            //Find charged pion that appears by decay of a primary particle
            if ((std::abs(g4_part_PDGcode->at(i_p)) == 211) & (g4_part_process->at(i_p) == "Decay") ){
                if (findPos(g4_part_trackID, g4_part_mother->at(i_p))==-1){
                }
                if (g4_part_process->at(findPos(g4_part_trackID, g4_part_mother->at(i_p))) == "primary"){
                    pipm_found = true;
                    break;
                }
            }
        }

        if (pipm_found){
            events_pipm.push_back(i_e);
        }
    }

    return events_pipm;

}

double vectorNorm(const std::vector<double>& vec) {
    double sum = 0.0;
    for (double val : vec) {
        sum += val * val;  
    }
    return std::sqrt(sum); 
}


void momentum_pipm(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_k0_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    //Define momentum components
    double px;
    double py;
    double pz;

    //Get events where k0->pi+pi-
    std::vector<int> events_pipm = find_events_pipm(event_tree);

    //Create canvas and histogram 
    TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    TH1F *h_pip = new TH1F("h_pip", "", 50, 0 , 2); 
    TH1F *h_pim = new TH1F("h_pim", "", 50, 0 , 2); 

    int i_e;

    //Loop over the number of events
    for (size_t i_v = 0; i_v < events_pipm.size(); ++i_v){
        i_e = events_pipm[i_v];

        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        for(int i_p = 0; i_p < size_g4; i_p++){
            if (std::abs(g4_part_PDGcode->at(i_p)) == 211){
                if ((g4_part_process->at(i_p) == "Decay") & (g4_part_process->at(findPos(g4_part_trackID, g4_part_mother->at(i_p))) == "primary")){

                    //Get momentum components;
                    px = g4_part_P0_X->at(i_p);
                    py = g4_part_P0_Y->at(i_p);
                    pz = g4_part_P0_Z->at(i_p);

                    //Distinguish pi+ form pi- and fill histograms
                    if (g4_part_PDGcode->at(i_p) == 211){
                        h_pip->Fill(vectorNorm({px,py,pz}));
                    } else if (g4_part_PDGcode->at(i_p) == -211){
                        h_pim->Fill(vectorNorm({px,py,pz}));
                    }
                }
            }
        }
    }


    
    //Draw histogram
    h_pip->SetLineColor(kBlue);
    h_pip->SetLineWidth(3);
    h_pip->SetFillColor(kBlue-9);
    h_pim->SetLineColor(kRed);
    h_pim->SetLineWidth(3);
    h_pim->SetFillColor(kRed-9);

    THStack* hs = new THStack("hs", ";P_{0} [GeV/c];Events");

    hs->Add(h_pip);
    hs->Add(h_pim);

    hs->Draw("hist");

    
    hs->GetXaxis()->SetTitleSize(0.06);
    hs->GetXaxis()->SetLabelSize(0.06);
    hs->GetYaxis()->SetTitleSize(0.06);
    hs->GetYaxis()->SetLabelSize(0.06);
    //hs->GetXaxis()->SetTitleOffset(1.5);


    //Add legend
    TLegend *legend_s = new TLegend(0.7, 0.75, 0.9, 0.9); 
    legend_s->AddEntry(h_pip, "#pi^{+}", "f");
    legend_s->AddEntry(h_pim, "#pi^{-}", "f");
    legend_s->Draw();

    //Modify margin
    c->SetLeftMargin(0.15);  
    c->SetRightMargin(0.1);  
    c->SetTopMargin(0.10);   
    c->SetBottomMargin(0.15);

    c->Update();


}