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
            cout << new_indent << "   PDGCode: " << gen_part_PDGcode->at(i_p) << " Mass: " << gen_part_mass->at(i_p) << " Energy: " << gen_part_E0->at(i_p)<< endl;
            //cout << new_indent << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(i_p) << ", " << gen_part_start_pos_Y->at(i_p) << ", " << gen_part_start_pos_Z->at(i_p)  << ")" << endl;
            //cout << new_indent << "   Start Momentum (x,y,z) : (" << gen_part_P0_X->at(i_p) << ", " << gen_part_P0_Y->at(i_p) << ", " << gen_part_P0_Z->at(i_p)  << ")" << endl;

            //Call function again
            //print_siblings_gen(gen_part_trackID->at(i_p), new_indent, event_tree, i_e);
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

void k0_res_showinfo(){

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

    int n_k0 = 0;

    bool first_res;


    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        first_res = true;
        
        //Loop over ALL gen particles in i_e-th event and show info if there are stable final state k0 created via resonance decay
        for(int j = 0; j < size_gen; j++){
            if (gen_part_statusCode->at(j) == 1){
                if ((gen_part_PDGcode->at(j) == 311) & (nu_interaction_mode == 1)){

                    //Loop over all particles and show info of first resonance and siblings
                    for(int k = 0; k < size_gen; k++){
                        if (((gen_part_PDGcode->at(k) > 1e5) & (gen_part_PDGcode->at(k) < 1e7)) & (first_res)){

                            cout << gen_part_PDGcode->at(k) << " mass: " << gen_part_mass->at(k) << endl;
                            first_res = false;

                            std::string indent = "";
                            
                            //Print resonance decay
                            //cout << endl << "=========================================" << endl;
                            //cout << endl << "===== EVENT " << i_e << " ===============================================" << endl;
                            //cout << "------ RESONANCE FOUND -------------------------- "<< endl;
                            //cout << "ID: " << gen_part_trackID->at(k) << " Status code: "<< gen_part_statusCode->at(k) <<  " Mother: " << gen_part_mother->at(k) << endl;
                            //cout << "   PDGCode: " << gen_part_PDGcode->at(k) << " Mass: " << gen_part_mass->at(k) << " Energy: " << gen_part_E0->at(k)<< endl;
                            //cout << "   Start Pos (x,y,z) : (" << gen_part_start_pos_X->at(k) << ", " << gen_part_start_pos_Y->at(k) << ", " << gen_part_start_pos_Z->at(k)  << ")" << endl;
                            
                            //print_siblings_gen(gen_part_trackID->at(k), indent, event_tree, i_e);
                            
                        }
                    }
                    n_k0++;

                }
            }
        }
    }
    cout << endl << n_k0 << " events with k0 from resonance" << endl;
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
3222        1.18937     SigmaP          uus             7
3122        1.11568     Lambda          uds             5
3212        1.19255     Sigma0          uds             4
*/

//NEXT: HISTOGRAMA 2D X->BARION MADRE, Y->BARION HERMANO