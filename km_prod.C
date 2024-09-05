#include "tree_utils.cpp"
#include "Includes.h"
#include <stdio.h>
#include <string.h>

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

void print_siblings(int ID, std::string indent, std::vector<int> *gen_part_trackID, std::vector<int> *gen_part_mother, std::vector<int> *gen_part_statusCode,std::vector<int> *gen_part_PDGcode){
    //Recursive function that prints all info about a particle siblings
    int sizevec = gen_part_trackID->size();

    std::string new_tab = "\t";

    //Add tabulation
    std::string new_indent = indent + new_tab;

    for (int j = 0; j < sizevec; j++){
        if (gen_part_mother->at(j) == ID){
            cout << new_indent << "----------------------------------------------------------------" << endl;
            cout << new_indent << "ID: " << gen_part_trackID->at(j) << " Status code: "<< gen_part_statusCode->at(j) <<  " Mother: " << gen_part_mother->at(j) << endl;
            cout << new_indent << "   PDGCode: " << gen_part_PDGcode->at(j) << endl;

            //Call function again
            print_siblings(j, new_indent, gen_part_trackID, gen_part_mother, gen_part_statusCode, gen_part_PDGcode);
        } 
    }
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
    //TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    //TH2F *h1 = new TH2F("h1", "K- smthng", 3, 0.5 , 3.5, 3, 0.5, 3.5); //x-> n of k+, y-> n of k-

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
                if (gen_part_PDGcode->at(j) == -321){

                    //Start reading the entry with index i_e
                    event_tree->GetEntry(i_e);

                    //Print relevant information about the tree
                    cout << "===== event " << event_ID << " ===============================================" << endl;
                    cout << "Kaon ID: " << gen_part_trackID->at(j) << endl << endl;

                    //Print relevant informationa about the neutrino that generated the interaction
                    cout << "Neutrino properties: " << endl;
                    cout << "PDG: " << nu_PDG << " Energy: " << nu_E0 << " weight: " << nu_weight << endl;
                    cout << "Interaction mode: " << nu_interaction_mode << " Interaction type: " <<  nu_interaction_type << " CCorNC: " << nu_CC_NC << endl;
                    cout << "Target: " << nu_target << " HitNuc: " << nu_HitNuc << " HitQuark: "<< nu_HitQuark << endl;
                    cout << "W: " <<  nu_W << " X: " << nu_X << " Y: "<< nu_Y << " Qsqr: "<< nu_QSqr << endl;
                    cout << endl;

                    //Print relevant information about the generator particles in the interaction
                    cout << "Generator Particles: " << endl;
                    //The number of particles is given by the size of any gen_part_* vector
                    int num_gen_particles = gen_part_E0->size();
                    for(int i_p = 0; i_p < num_gen_particles; i_p++) {
                        std::string indent = "";

                        //Only print here if it has no mother
                        if (gen_part_mother->at(i_p) == -1){
                            cout << "ID: " << gen_part_trackID->at(i_p) << " Status code: "<< gen_part_statusCode->at(i_p) <<  " Mother: " << gen_part_mother->at(i_p) << endl;
                            cout << "   PDGCode: " << gen_part_PDGcode->at(i_p) << endl;//<< " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
                            //cout << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
                            //cout << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;

                            //Call print_siblings
                            print_siblings(i_p, indent, gen_part_trackID, gen_part_mother, gen_part_statusCode, gen_part_PDGcode);
                        }


                    }
                    
                    cout << endl << endl;
                    
                    
                    /*
                    int part;
                    int mother;
                    int grandmother;

                    part = gen_part_trackID->at(j);
                    mother = gen_part_mother->at(j);
                    grandmother = gen_part_mother->at(mother);


                    cout << "EVENTO " << i_e << endl;
                    cout << "ID " << part << endl;
                    cout << "mother ID " << mother << endl;
                    cout << "mother PDG code " << gen_part_PDGcode->at(mother) << endl;
                    cout << "mother status " << gen_part_statusCode->at(mother) << endl;
                    cout << "grandmother PDG code " << gen_part_PDGcode->at(grandmother) << endl;
                    cout << "grandmother status " << gen_part_statusCode->at(grandmother) << endl;
                    cout << endl;
                    cout << endl;
                    */ 

                    /*
                    for (int k=0; k<sizevec; k++){
                        cout << k << "   " << gen_part_PDGcode->at(k) << endl;
                    }
                    cout << endl;
                    */
                }
            }
        }
    }
}