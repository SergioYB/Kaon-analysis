#include "tree_utils.cpp"
#include "Includes.h"
#include "read_event_tree.C"
#include "read_subrun_tree.C"

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


//Recursive function that prints all info about a particle siblings
void print_siblings_gen(int ID, std::string indent, TTree* event_tree, int i_e){
    //Obtain current event from event_tree
    event_tree->GetEntry(i_e);

    int size_gen = gen_part_trackID->size();

    std::string new_tab = "\t";

    //Add tabulation
    std::string new_indent = indent + new_tab;

    for (int i_p = 0; i_p < size_gen; i_p++){
        if (gen_part_mother->at(i_p) == ID){
            cout << new_indent << "----------------------------------------------------------------" << endl;
            cout << new_indent << "ID: " << gen_part_trackID->at(i_p) << " Status code: "<< gen_part_statusCode->at(i_p) <<  " Mother: " << gen_part_mother->at(i_p) << endl;
            cout << new_indent << "   PDGCode: " << gen_part_PDGcode->at(i_p) << " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
            cout << new_indent << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;

            //Call function again
            print_siblings_gen(gen_part_trackID->at(i_p), new_indent, event_tree, i_e);
        } 
    }
}


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
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(i_p) << ", " << g4_part_P0_Y->at(i_p) << ", " << g4_part_P0_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(i_p) << ", " << g4_part_Pf_Y->at(i_p) << ", " << g4_part_Pf_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "   Process: " << g4_part_process->at(i_p) << endl;
            cout << new_indent << "   End_process: " << g4_part_end_process->at(i_p) << endl;
            //Call function again
            print_siblings_g4(g4_part_trackID->at(i_p), new_indent, event_tree, i_e);
        } 
    }
}


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

void k0_all_info(){

    TTree *event_tree;
    TTree *subrun_tree;

    event_tree = (TTree*)read_event_tree();
    subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;


    //Create canvas and histogram 
        //TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
        //TH1F *h1 = new TH1F("h1", "Ks vs Kl", 2, 0.5 , 2.5); //x-> type of K0, y-> counts

    int n_k0;
    int n_ak0;
    int n_ks;
    int n_kl;
    int n_event_wk0 = 0;

    bool k0_found;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        n_k0 = 0;
        n_ak0 = 0;
        n_ks = 0;
        n_kl = 0;

        k0_found = false;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if ((gen_part_PDGcode->at(j) == 311 || gen_part_PDGcode->at(j) == -311) & (!k0_found)){
                    n_event_wk0++;
                    k0_found = true;

                    //Loop over gen particles and show info indenting in parenting

                    //Print relevant information about the tree
                    cout << "===== event " << i_e << " ===============================================" << endl;
                    cout << "Kaon ID: " << gen_part_trackID->at(j) << endl << endl;

                    //Print relevant informationa about the neutrino that generated the interaction
                    cout << "Neutrino properties: " << endl;
                    cout << "PDG: " << nu_PDG << " Energy: " << nu_E0 << " weight: " << nu_weight << endl;
                    cout << "Interaction mode: " << nu_interaction_mode << " Interaction type: " <<  nu_interaction_type << " CCorNC: " << nu_CC_NC << endl;
                    cout << "Target: " << nu_target << " HitNuc: " << nu_HitNuc << " HitQuark: "<< nu_HitQuark << endl;
                    cout << "W: " <<  nu_W << " X: " << nu_X << " Y: "<< nu_Y << " Qsqr: "<< nu_QSqr << endl;
                    cout << endl;

                    //Print relevant information about the generator particles in the interaction
                    cout << "----------- Generator Particles: ------------------------------------" << endl;
                    //The number of particles is given by the size of any gen_part_* vector
                    int num_gen_particles = gen_part_E0->size();
                    for(int i_p = 0; i_p < num_gen_particles; i_p++) {
                        std::string indent = "";

                        //Only print here if it has no mother
                        if (gen_part_mother->at(i_p) == -1){
                            cout << "----------------------------------------------------------------" << endl;
                            cout << "ID: " << gen_part_trackID->at(i_p) << " Status code: "<< gen_part_statusCode->at(i_p) <<  " Mother: " << gen_part_mother->at(i_p) << endl;
                            cout << "   PDGCode: " << gen_part_PDGcode->at(i_p) << endl; // << " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
                            cout << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
                            //cout << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;

                            //Call print_siblings_gen
                            print_siblings_gen(gen_part_trackID->at(i_p), indent, event_tree, i_e);
                        }
                    }

                    //Print relevant information about the particles in the interaction once they have been propagated by Geant4
                    cout << "------------- G4 Particles: ----------------------------" << endl;
                    
                    for(int i_p = 0; i_p < size_g4; i_p++) {

                        std::string indent = "";

                        //Print only if mother = 0 an it is a k0 (s/l)
                        if ((g4_part_mother->at(i_p) == 0) & (g4_part_PDGcode->at(i_p) == 311 || g4_part_PDGcode->at(i_p) == -311 || g4_part_PDGcode->at(i_p) == 310 || g4_part_PDGcode->at(i_p) == 130)){
                            cout << "----------------------------------------------------------------" << endl;
                            cout << "ID: " << g4_part_trackID->at(i_p) << " Mother: " << g4_part_mother->at(i_p) << endl;
                            cout << "   PDGCode: " << g4_part_PDGcode->at(i_p) << " Mass: " << g4_part_mass->at(i_p) << " Initial Energy: " << g4_part_E0->at(i_p) << " Final Energy: " << g4_part_Ef->at(i_p) << endl;
                            cout << "   Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(i_p) << ", " << g4_part_start_pos_Y->at(i_p) << ", " << g4_part_start_pos_Z->at(i_p)  << ")" << endl;
                            cout << "   End Pos (x,y,z) : (" << g4_part_end_pos_X->at(i_p) << ", " << g4_part_end_pos_Y->at(i_p) << ", " << g4_part_end_pos_Z->at(i_p)  << ")" << endl;
                            cout << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(i_p) << ", " << g4_part_P0_Y->at(i_p) << ", " << g4_part_P0_Z->at(i_p)  << ")" << endl;
                            cout << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(i_p) << ", " << g4_part_Pf_Y->at(i_p) << ", " << g4_part_Pf_Z->at(i_p)  << ")" << endl;
                            cout << "   Process: " << g4_part_process->at(i_p) << endl;
                            cout << "   End_process: " << g4_part_end_process->at(i_p) << endl;cout << "ID: " << g4_part_trackID->at(i_p) << " Mother: " << g4_part_mother->at(i_p) << endl;
                            cout << "   PDGCode: " << g4_part_PDGcode->at(i_p) << " Mass: " << g4_part_mass->at(i_p) << " Initial Energy: " << g4_part_E0->at(i_p) << " Final Energy: " << g4_part_Ef->at(i_p) << endl;
                            cout << "   Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(i_p) << ", " << g4_part_start_pos_Y->at(i_p) << ", " << g4_part_start_pos_Z->at(i_p)  << ")" << endl;
                            cout << "   End Pos (x,y,z) : (" << g4_part_end_pos_X->at(i_p) << ", " << g4_part_end_pos_Y->at(i_p) << ", " << g4_part_end_pos_Z->at(i_p)  << ")" << endl;
                            cout << "   Start Momentum (x,y,z) : (" << g4_part_P0_X->at(i_p) << ", " << g4_part_P0_Y->at(i_p) << ", " << g4_part_P0_Z->at(i_p)  << ")" << endl;
                            cout << "   End Momentum (x,y,z) : (" << g4_part_Pf_X->at(i_p) << ", " << g4_part_Pf_Y->at(i_p) << ", " << g4_part_Pf_Z->at(i_p)  << ")" << endl;
                            cout << "   Process: " << g4_part_process->at(i_p) << endl;
                            cout << "   End_process: " << g4_part_end_process->at(i_p) << endl;

                            //Call print_siblings_g4
                            print_siblings_g4(g4_part_trackID->at(i_p), indent, event_tree, i_e);
                        }
                    }
                    cout << endl;
                    cout << endl << endl;
                }
            }
        }
    }
    cout << endl << endl << "There are " << n_event_wk0 << " events with k0" << endl;
}