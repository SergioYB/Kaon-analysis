#include "tree_utils.cpp"
#include "Includes.h"

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

void k0_slratio(){
    //Definition of the file from which the tree is readed and the folder inside the ".root" file where the tree is stored
    TFile *input_file;
    TDirectoryFile *tree_dir;

    //Definition of the tree that stores information about each LArSoft event
    TTree *event_tree;
    TTree *subrun_tree;

    //Definition of the path where the file containing the truth tree is stored, along with the code that opens the file and both the subrun and the event by event tree
    string path_to_tree = "analysis_output_truth_BNB_single_100k_events.root";
    input_file = new TFile(path_to_tree.c_str());
    tree_dir = (TDirectoryFile*)input_file->Get("ana");
    event_tree =(TTree*)tree_dir->Get("tree");
    subrun_tree =(TTree*)tree_dir->Get("subrun_tree");


    //Code that sets the Branches fot the subrun and event trees in order to use the information they store, please visit "tree_utils.cpp" to see how the functions are defined
    set_branch(event_tree);
    set_branch_subtree(subrun_tree);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    //Number of K0+- in each event
    int n_k0;
    int n_ks;
    int n_kl;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h1 = new TH1F("h1", "Ks vs Kl", 2, 0.5 , 2.5); //x-> type of K0, y-> counts

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


        /*
        for(int j = 0; j < size_gen; j++){
            if(gen_part_PDGcode->at(j) == 311){
                cout << j << "    " << gen_part_trackID->at(j) << endl;
            }
        }
        */

       //cout << g4_part_trackID->at(0) << endl;
        
        int j_k0;
        int j_ks;
        int j_kl;

        bool are_kaon = false;
        
        //Loop over ALL particles in i-th event
        for(int j = 0; j < size_g4; j++){
            if (g4_part_PDGcode->at(j) == 311){
                if (!are_kaon){
                    cout << endl;
                    cout << " -------- EVENT " << i_e << " -------------- " << endl;
                    are_kaon = true;
                }
                n_k0++;
                cout << "k0 process: " << g4_part_process->at(j) << endl;
                cout << "\t k0g4ID: " << g4_part_trackID->at(j) << endl;
                cout << "\t k0genID: " << gen_part_trackID->at(j) << endl;
                cout << "\t k0 mother: " << g4_part_mother->at(j) << endl;
                cout << "\t mother-th gen particle ID: " << gen_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                cout << "\t mother-th g4 particle ID: " << g4_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                cout << "\t End Pos (x,y,z) : (" << g4_part_end_pos_X->at(j) << ", " << g4_part_end_pos_Y->at(j) << ", " << g4_part_end_pos_Z->at(j)  << ")" << endl;
            }
            if (g4_part_PDGcode->at(j) == 130){
                if (!are_kaon){
                    cout << endl;
                    cout << " -------- EVENT " << i_e << " -------------- " << endl;
                    are_kaon = true;
                }
                //cout << " -------- EVENT " << i_e << " -------------- " << endl;
                n_kl++;
                cout << "kl process: " << g4_part_process->at(j) << endl;
                cout << "\t klID: " << g4_part_trackID->at(j) << endl;
                cout << "\t kl mother: " << g4_part_mother->at(j) << endl;
                cout << "\t Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(j) << ", " << g4_part_start_pos_Y->at(j) << ", " << g4_part_start_pos_Z->at(j)  << ")" << endl;
                if (n_k0 == 0){
                    cout << "================ Kl WITHOUT K0 AT EVENT " << i_e << "======================" << endl;
                    cout << "mother PDG (at motherID): " << g4_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                    cout << "mother PDG (at motherID-1): " << g4_part_PDGcode->at(g4_part_mother->at(j)-1) << endl;

                }
            }
            if (g4_part_PDGcode->at(j) == 310){
                if (!are_kaon){
                    cout << endl;
                    cout << " -------- EVENT " << i_e << " -------------- " << endl;
                    are_kaon = true;
                }
                //cout << " -------- EVENT " << i_e << " -------------- " << endl;
                n_ks++;
                cout << "ks process: " << g4_part_process->at(j) << endl;
                cout << "\t ksID: " << g4_part_trackID->at(j) << endl;
                cout << "\t ks mother: " << g4_part_mother->at(j) << endl;
                cout << "\t Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(j) << ", " << g4_part_start_pos_Y->at(j) << ", " << g4_part_start_pos_Z->at(j)  << ")" << endl;
                if (n_k0 == 0){
                    cout << "================ Ks WITHOUT K0 AT EVENT " << i_e << "======================" << endl;
                    cout << "mother PDG (at motherID): " << g4_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                    cout << "mother PDG (at motherID-1): " << g4_part_PDGcode->at(g4_part_mother->at(j)-1) << endl;
                }
            }
        }

        /*
        if ((n_ks >0) & (n_k0 == 0)){
            cout << " ================= HAY Ks SIN k0 EN EVENTO " << i_e << "================== " << endl;
            cout << "N ks: " << n_ks << endl;
            cout << "Mother: " << g4_part_mother->at(j_ks) << endl;
            cout << "Mother PDG code: " << g4_part_PDGcode->at(g4_part_mother->at(j_ks)) << endl;
        }

        if ((n_kl >0) & (n_k0 == 0)){
            cout << " ================= HAY Kl SIN k0 EN EVENTO " << i_e << "================== " << endl;
            cout << "N kl " << n_kl << endl;
            cout << "Mother: " << g4_part_mother->at(j_kl) << endl;
            cout << "Mother PDG code: " << g4_part_PDGcode->at(g4_part_mother->at(j_kl)) << endl;
        }
        */

        /*
        if (n_k0 > 0 || n_ks > 0 || n_kl > 0){
            cout << endl;
        }
        */
        
        

        /*
        if (n_kl > 0 || n_kl > 0){
            cout << "======= EVENTO " << i_e << " ===========" << endl; 
            cout << "N of K0s: " << n_ks << endl;
            cout << "N of k0l: " << n_kl << endl << endl;
        }
        */
    }


}