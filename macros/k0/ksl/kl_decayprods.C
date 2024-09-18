#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"

void print_siblings_g4(int ID, std::string indent, TTree* event_tree, int i_e){
    //Recursive function that prints all info about a particle siblings
    //Obtain current event from event_tree
    event_tree->GetEntry(i_e);

    int size_g4 = g4_part_trackID->size();

    std::string new_tab = "\t";

    //Add tabulation
    std::string new_indent = indent + new_tab;

    for (int i_p = 0; i_p < size_g4; i_p++){
        if (g4_part_mother->at(i_p) == ID){
            cout << new_indent << "----------------------------------------------------------------" << endl;
            cout << new_indent << "ID: " << g4_part_trackID->at(i_p) << " Mother: " << g4_part_mother->at(i_p) << endl;
            cout << new_indent << "   PDGCode: " << g4_part_PDGcode->at(i_p) << " Mass: " << g4_part_mass->at(i_p) << " Initial Energy: " << g4_part_E0->at(i_p) << " Final Energy: " << g4_part_Ef->at(i_p) << endl;
            cout << new_indent << "   Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(i_p) << ", " << g4_part_start_pos_Y->at(i_p) << ", " << g4_part_start_pos_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   End Pos (x,y,z) : (" << g4_part_end_pos_X->at(i_p) << ", " << g4_part_end_pos_Y->at(i_p) << ", " << g4_part_end_pos_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(i_p) << ", " << g4_part_P0_Y->at(i_p) << ", " << g4_part_P0_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(i_p) << ", " << g4_part_Pf_Y->at(i_p) << ", " << g4_part_Pf_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   Process: " << g4_part_process->at(i_p) << endl;
            cout << new_indent << "   End_process: " << g4_part_end_process->at(i_p) << endl;
            //Call function again
            print_siblings_g4(g4_part_trackID->at(i_p), new_indent, event_tree, i_e);
        } 
    }
}

bool ends_inside_detector(TTree* event_tree, int i_p, double fv){
    // returns true if particle end pos is inside detector
    double x = g4_part_end_pos_X->at(i_p);
    double y = g4_part_end_pos_Y->at(i_p);
    double z = g4_part_end_pos_Z->at(i_p);

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}

bool starts_inside_detector(TTree* event_tree, int i_p, double fv){
    // returns true if particle start pos is inside detector
    double x = g4_part_start_pos_X->at(i_p);
    double y = g4_part_start_pos_Y->at(i_p);
    double z = g4_part_start_pos_Z->at(i_p);

    //return ((x >= -200) & (x <= 200) & (y >= -200) & (y <= 200) & (z >= 0) & (z <= 500));
    return ((x >= -200 + fv) & (x <= 200 - fv) & (y >= -200 + fv) & (y <= 200 - fv) & (z >= 0 + fv) & (z <= 500 - fv)); //FV
}

void kl_decayprods(){

    TTree *event_tree;
    TTree *subrun_tree;

    double fv = 10;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;    
    
    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();


        //Loop over g4 particles to finds primary k0 created inside detector
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (starts_inside_detector(event_tree, i_p, fv))){

                //Loop over g4 particles to find k0s or l and check if it is inside or outside
                for (int j_p = 0; j_p < size_g4; j_p++){

                    //Find kl
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_PDGcode->at(j_p) == 130)){
                        if (ends_inside_detector(event_tree, j_p, fv)){
                            if (g4_part_end_process->at(j_p) == "Decay"){
                                //Show children info
                                cout << endl << "=============== EVENT " << i_e << " ===================" << endl;
                                for(int k_p = 0; k_p < size_g4; k_p++) {

                                    if ((g4_part_mother->at(k_p) == g4_part_trackID->at(j_p)) & (g4_part_process->at(k_p) == "Decay")){// & (g4_part_PDGcode->at(i_p) == 14 || g4_part_PDGcode->at(i_p) == 311 || g4_part_PDGcode->at(i_p) == -311 || g4_part_PDGcode->at(i_p) == 310 || g4_part_PDGcode->at(i_p) == 130)){
                                        
                                        cout << "----------------------------------------------------------------" << endl;
                                        cout << "ID: " << g4_part_trackID->at(k_p) << " Mother: " << g4_part_mother->at(k_p) << endl;
                                        cout << "   PDGCode: " << g4_part_PDGcode->at(k_p) << " Mass: " << g4_part_mass->at(k_p) << " Initial Energy: " << g4_part_E0->at(k_p) << " Final Energy: " << g4_part_Ef->at(k_p) << endl;
                                        cout << "   Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(k_p) << ", " << g4_part_start_pos_Y->at(k_p) << ", " << g4_part_start_pos_Z->at(k_p)  << ")" << endl;
                                        cout << "   End Pos (x,y,z) : (" << g4_part_end_pos_X->at(k_p) << ", " << g4_part_end_pos_Y->at(k_p) << ", " << g4_part_end_pos_Z->at(k_p)  << ")" << endl;
                                        cout << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(k_p) << ", " << g4_part_P0_Y->at(k_p) << ", " << g4_part_P0_Z->at(k_p)  << ")" << endl;
                                        cout << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(k_p) << ", " << g4_part_Pf_Y->at(k_p) << ", " << g4_part_Pf_Z->at(k_p)  << ")" << endl;
                                        cout << "   Process: " << g4_part_process->at(k_p) << endl;
                                        cout << "   End_process: " << g4_part_end_process->at(k_p) << endl;
                                    

                                        //Call print_siblings_g4
                                        //print_siblings_g4(g4_part_trackID->at(i_p), indent, event_tree, i_e);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}