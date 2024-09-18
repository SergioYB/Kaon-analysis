#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"

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
            //cout << new_indent << "----------------------------------------------------------------" << endl;
            //cout << new_indent << "ID: " << gen_part_trackID->at(i_p) << " Status code: "<< gen_part_statusCode->at(i_p) <<  " Mother: " << gen_part_mother->at(i_p) << endl;
            //cout << new_indent << "   PDGCode: " << gen_part_PDGcode->at(i_p) << " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
            //cout << new_indent << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;
            cout << new_indent << "PDGCode: " << gen_part_PDGcode->at(i_p) << endl;
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
            cout << new_indent << "\t   Process: " << g4_part_process->at(i_p) << endl;
            cout << new_indent << "\t   End_process: " << g4_part_end_process->at(i_p) << endl;
            //Call function again
            //print_siblings_g4(g4_part_trackID->at(i_p), new_indent, event_tree, i_e);
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

void res_baryons_info(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    subrun_tree = (TTree*)read_subrun_tree(data_dir);

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;
    bool first_res;

    //Create canvas and histogram 
        //TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
        //TH1F *h1 = new TH1F("h1", "Ks vs Kl", 2, 0.5 , 2.5); //x-> type of K0, y-> counts


    bool k0_found;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        first_res = true;

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((std::abs(g4_part_PDGcode->at(i_p)) == 311) & (g4_part_process->at(i_p) == "primary") & (nu_interaction_mode == 1)){


                //Loop over gen particles to find resonances
                for (int j_p = 0; j_p < size_gen; j_p++){
                    if ((gen_part_PDGcode->at(j_p) > 1e5) & (gen_part_PDGcode->at(j_p) < 1e7)){ 
                        cout << endl << "=========================" << endl;
                        cout << "RESONANCE FOUND: " << gen_part_PDGcode->at(j_p)<< " in EVENT: " << i_e << endl;
                        for (int k_p = 0; k_p<size_gen; k_p++){
                            if (gen_part_mother->at(k_p) == gen_part_trackID->at(j_p)){
                                cout << "\t PDG: " << gen_part_PDGcode->at(k_p) << endl;
                            }
                        }
                        cout << endl << "--------- GEN PARTICLES -----------------------" << endl;
                        std::string indent = "";
                        for (int k_p = 0; k_p<size_gen; k_p++){
                            if (gen_part_mother->at(k_p) == -1){
                                cout << "PDGCode: " << gen_part_PDGcode->at(i_p) << endl;
                                print_siblings_gen(gen_part_trackID->at(k_p), indent, event_tree, i_e);
                            }
                        }
                    }
                }

                //Loop over g4 particles and show primary
                for (int j_p = 0; j_p < size_g4; j_p++){
                    if ((g4_part_mother->at(j_p) == g4_part_mother->at(i_p)) & (g4_part_process->at(j_p) == "primary")){ //Both conditions equivalent?
                        cout << "PDG: " << g4_part_PDGcode->at(j_p) << endl;
                    }
                }
            }
        }
    }
}

/*RESONANCIES OBSERVED 

CODE        MASS        NAME            QUARK_COMP      AMOUNT (16 kaon from resonance)
212212      1.71        N+(1710)        uud   
202214      1.72        N+(1720)        uud  
212112      1.71        N0(1710)        udd         
202114      1.72        N0(1720)        udd            
212114      1.57        Delta0(1600)    udd             
222114      1.92        Delta0(1920)    udd             
*/

/*BROTHERS OBSERVED
CODE        MASS        NAME            QUARK_COMP      AMOUNT (16 kaon from resonance)
3222        1.18937     Sigma+          uus             7
3212        1.19255     Sigma0          uds             4
3122        1.11568     Lambda          uds             5
*/