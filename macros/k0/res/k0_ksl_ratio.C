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

void k0_ksl_ratio(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    //Number of K0+- in each event
    int n_k0;
    int n_ks;
    int n_kl;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h1 = new TH1F("h1", "", 2, 0.5 , 2.5); //x-> ks or kl

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        //Set number of kaons to 0
        n_k0 = 0;
        n_ks = 0;
        n_kl = 0;


        //Loop over g4 particles to finds primary k0 produced by resonance decay
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (nu_interaction_mode == 1)){
                n_k0++;
                //Print k0 g4 info
                cout << endl << "========== EVENT " << i_e << " ================" << endl;
                cout << "k0 process: " << g4_part_process->at(i_p) << endl;
                cout << " k0g4ID: " << g4_part_trackID->at(i_p) << endl;
                cout << " k0 mother: " << g4_part_mother->at(i_p) << endl;
                cout << " Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(i_p) << ", " << g4_part_start_pos_Y->at(i_p) << ", " << g4_part_start_pos_Z->at(i_p)  << ")" << endl;
                cout << " End Pos (x,y,z) : (" << g4_part_end_pos_X->at(i_p) << ", " << g4_part_end_pos_Y->at(i_p) << ", " << g4_part_end_pos_Z->at(i_p)  << ")" << endl;

                //Loop over g4 particles to find k0 children
                for (int j_p = 0; j_p < size_g4; j_p++){
                    if (g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)){

                        //Print children info
                        cout << endl;
                        cout << "\t Chldrn PDG: " << g4_part_PDGcode->at(j_p) << endl;
                        cout << "\t  Chldrn process: " << g4_part_process->at(j_p) << endl;
                        cout << "\t  Chldrn g4ID: " << g4_part_trackID->at(j_p) << endl;
                        cout << "\t  Chldrn mother: " << g4_part_mother->at(j_p) << endl;
                        cout << "\t  Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(j_p) << ", " << g4_part_start_pos_Y->at(j_p) << ", " << g4_part_start_pos_Z->at(j_p)  << ")" << endl;
                        cout << "\t  End Pos (x,y,z) : (" << g4_part_end_pos_X->at(j_p) << ", " << g4_part_end_pos_Y->at(j_p) << ", " << g4_part_end_pos_Z->at(j_p)  << ")" << endl;
                        
                        //Fill histogram
                        if (g4_part_PDGcode->at(j_p) == 310){
                            h1->Fill(1);
                        } else if (g4_part_PDGcode->at(j_p) == 130){
                            h1->Fill(2);
                        }
                    }
                }
            }
        }
        
    }

    double n_POT = get_total_POT(subrun_tree);

    //Draw histogram
    h1->SetStats(0);

    h1->GetXaxis()->SetBinLabel(1, "K^{0}_{S}");
    h1->GetXaxis()->SetBinLabel(2, "K^{0}_{L}");

    h1->GetXaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetLabelSize(0.07);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);

    h1->GetXaxis()->SetTitle("K^{0} flavour");
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

}