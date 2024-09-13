#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"


bool ends_inside_detector(TTree* event_tree, int i_p){
    // returns true if particle end pos is inside detector
    double x = g4_part_end_pos_X->at(i_p);
    double y = g4_part_end_pos_Y->at(i_p);
    double z = g4_part_end_pos_Z->at(i_p);

    double fv = 20;

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}

bool starts_inside_detector(TTree* event_tree, int i_p){
    // returns true if particle start pos is inside detector
    double x = g4_part_start_pos_X->at(i_p);
    double y = g4_part_start_pos_Y->at(i_p);
    double z = g4_part_start_pos_Z->at(i_p);

    double fv = 20;

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}


void ks_kl_contenido(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    //Create canvas and histogram 
    TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    TH2F *hist = new TH2F("hist", "", 2, 0.5 , 2.5, 2, 0.5, 2.5); //x-> ks or kl, y->in or out

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();


        //Loop over g4 particles to finds primary k0 created inside detector
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (starts_inside_detector(event_tree,i_p))){

                //Loop over g4 particles to find k0s or l and check if it is inside or outside
                for (int j_p = 0; j_p < size_g4; j_p++){

                    //Find ks
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_PDGcode->at(j_p) == 310)){
                        if (ends_inside_detector(event_tree, j_p)){
                            hist->Fill(1,1);
                        } else {
                            hist->Fill(1,2);
                        }

                    }

                    //Find kl
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_PDGcode->at(j_p) == 130)){
                        if (ends_inside_detector(event_tree, j_p)){
                            hist->Fill(2,1);
                        } else {
                            hist->Fill(2,2);
                        }

                    }
                }
            }
        }
        
    }

    //Draw histogram
    hist->SetStats(0);

    hist->GetXaxis()->SetBinLabel(1, "K^{0}_{S}");
    hist->GetXaxis()->SetBinLabel(2, "K^{0}_{L}");
    hist->GetYaxis()->SetBinLabel(1, "Inside");
    hist->GetYaxis()->SetBinLabel(2, "Outside");

    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.06);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.06);

    hist->GetXaxis()->SetTitle("K^{0} flavour");
    hist->GetYaxis()->SetTitle("End position");

    //Modify margin
    c->SetLeftMargin(0.15);  
    c->SetRightMargin(0.1);  
    c->SetTopMargin(0.10);   
    c->SetBottomMargin(0.15);

    //Set color palette 
    gStyle->SetPalette(kViridis);
    

    //Add text
    TLatex latex;
    TLatex latex_white;

    latex_white.SetTextColor(kWhite);

    hist->Draw("COLZ");

    latex.DrawLatex(hist->GetXaxis()->GetBinCenter(1), hist->GetYaxis()->GetBinCenter(1), Form("%.0f", hist->GetBinContent(1,1)));
    latex.DrawLatex(hist->GetXaxis()->GetBinCenter(2), hist->GetYaxis()->GetBinCenter(1), Form("%.0f", hist->GetBinContent(2,1)));
    latex_white.DrawLatex(hist->GetXaxis()->GetBinCenter(2), hist->GetYaxis()->GetBinCenter(2), Form("%.0f", hist->GetBinContent(2,2)));
    latex_white.DrawLatex(hist->GetXaxis()->GetBinCenter(1), hist->GetYaxis()->GetBinCenter(2), Form("%.0f", hist->GetBinContent(1,2)));

    c->Update();
}