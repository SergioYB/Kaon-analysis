include "tree_utils.cpp"
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


void km_prod(){
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
    float sizevec; // size of the vector containing all particles of each event

    //Number of K0+- in each event
    int n_kp;
    int n_km;

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH2F *h1 = new TH2F("h1", "K- smthng", 3, 0.5 , 3.5, 3, 0.5, 3.5); //x-> n of k+, y-> n of k-

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        sizevec = gen_part_trackID->size();

        //Set number of kaons to 0
        n_kp = 0;
        n_km = 0;

        //Loop over ALL particles in i-th event
        for(int j = 0; j < sizevec; j++){
            if (gen_part_statusCode->at(j) == 1){
                if (gen_part_PDGcode->at(j) == 321){
                    n_kp++;
                } else if (gen_part_PDGcode->at(j) == -321){
                    n_km++;
                }
            }
        }
    }
}