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

/*
******* Classifying the amount of k0s and k0l in k0 production events *****
*/

void k0_prod(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    int n_res = 0;
    int n_discc = 0;
    int n_disnc = 0;

    bool k0_found;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h1 = new TH1F("h1", "", 3, 0.5 , 3.5); //x-> interaction mode

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        k0_found = false;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if (gen_part_PDGcode->at(j) == 311){
                    if (nu_interaction_mode == 1){
                        n_res++;
                    } else if (nu_interaction_mode == 2){
                        if (nu_interaction_type == 1091){
                            n_discc++;
                        } else if (nu_interaction_type == 1092){
                            n_disnc++;
                        }
                    } else{
                        cout << "Something else" << endl;
                    }
                }
            }
        }
    }

    //Add values to hist
    h1->Fill(1, n_res);
    h1->Fill(2,n_discc);
    h1->Fill(3,n_disnc);

    //Scaling histogram for one year production (3y = 10e21 POT)
    double n_POT = get_total_POT(subrun_tree);

    //DRAW h1 ===========================================================================

    h1->SetStats(0);

    h1->GetXaxis()->SetBinLabel(1, "RES");
    h1->GetXaxis()->SetBinLabel(2, "DIS CC");
    h1->GetXaxis()->SetBinLabel(3, "DIS NC");

    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);

    h1->GetXaxis()->SetTitle("Interaction mode");
    h1->GetYaxis()->SetTitle("Number of events");

    //h1->Scale(1e21/(3 * n_POT));

    h1->SetMinimum(0);

    //Add text
    TLatex latex;
    double total = h1->Integral();
    double tot_res = h1->GetBinContent(1);
    double tot_discc = h1->GetBinContent(2);
    double tot_disnc = h1->GetBinContent(3);

    //Color settings

    h1->SetLineColor(kRed);
    h1->SetLineWidth(3);

    h1->SetFillColorAlpha(kRed-9, 0.3);
    h1->SetFillStyle(1001); 

    h1->Draw("HIST");

    latex.DrawLatex(0.8, tot_res * 0.8, Form("%.2f%%", tot_res/total*100)); //Draw text
    latex.DrawLatex(1.8, tot_discc * 0.9, Form("%.2f%%", tot_discc/total*100)); //Draw text
    latex.DrawLatex(2.8, tot_disnc * 0.8, Form("%.2f%%", tot_disnc/total*100)); //Draw text

    c1->Update();

    cout << h1->GetBinContent(1)<< endl;
    cout << h1->GetBinContent(2)<< endl;
    cout << h1->GetBinContent(3)<< endl;
    
}