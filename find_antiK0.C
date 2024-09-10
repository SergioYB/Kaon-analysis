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

void find_antiK0(){

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

    bool k0_found;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        n_k0 = 0;
        n_ak0 = 0;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if (gen_part_PDGcode->at(j) == 311){
                    n_k0++;
                } else if (gen_part_PDGcode->at(j) == -311){
                    n_ak0++;
                }
            }
        }

        /*
        if ((n_k0 == 1) & (n_ak0 == 0)){
            cout << "No anti-K0" << endl;
            cout << "nu interaction mode: " << nu_interaction_mode << endl;
            cout << "nu interaction type: " << nu_interaction_type << endl << endl;
        }
        */

        if ((n_k0 == 1) & (n_ak0 == 1)){
            cout << "Anti-K0 found" << endl;
            cout << "nu interaction mode: " << nu_interaction_mode << endl;
            cout << "nu interaction type: " << nu_interaction_type << endl << endl;
        }
    }
}