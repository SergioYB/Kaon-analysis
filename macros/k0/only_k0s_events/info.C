#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"



void info(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_k0_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    //Number of K0+- in each event
    int n_k0;
    int n_ks;
    int n_kl;

    //Create canvas and histogram 
    //TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    //TH1F *h1 = new TH1F("h1", "", 2, 0.5 , 2.5); //x-> ks or kl

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

        //Loop over gen particles to finds primary k0S produced by resonance decay
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 310) & (g4_part_process->at(i_p) == "primary") & (g4_part_end_process->at(i_p) == "Decay")){
                n_k0++;
                //Print k0 g4 info
                cout << endl << "========== EVENT " << i_e << " ================" << endl;
                cout << "k0 process: " << g4_part_process->at(i_p) << endl;
                cout << "k0 end process: " << g4_part_end_process->at(i_p) << endl;
                cout << "PDG: " << g4_part_PDGcode->at(i_p) << endl;
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
                    }
                }
            }
        }
    }
}