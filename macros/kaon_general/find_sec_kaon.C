#include "tree_utils.cpp"
#include "Includes.h"
#include "read_event_tree.C"
#include "read_subrun_tree.C"

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

/*
******* Classifying the amount of k0s and k0l in k0 production events *****
*/

void find_sec_kaon(){

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
    int n_ks;
    int n_kl;

    //Set number of kaons to 0
    int t_ks = 0;
    int t_kl = 0;
    int t_ks_nok0 = 0;
    int t_kl_nok0 = 0;

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

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
        
        //int j_k0;
        //int j_ks;
        //int j_kl;

        bool are_kaon = false;
        
        //Loop over ALL particles in i-th event
        //Show info only if there are ks or kl without k0 
        for(int j = 0; j < size_g4; j++){
            if (g4_part_PDGcode->at(j) == 311){

                n_k0++;

                /*
                if (!are_kaon){
                    cout << endl;
                    cout << " -------- EVENT " << i_e << " -------------- " << endl;
                    are_kaon = true;
                }*/
                /*
                cout << "k0 process: " << g4_part_process->at(j) << endl;
                cout << "\t k0g4ID: " << g4_part_trackID->at(j) << endl;
                cout << "\t k0genID: " << gen_part_trackID->at(j) << endl;
                cout << "\t k0 mother: " << g4_part_mother->at(j) << endl;
                cout << "\t mother-th gen particle ID: " << gen_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                cout << "\t mother-th g4 particle ID: " << g4_part_PDGcode->at(g4_part_mother->at(j)) << endl;
                cout << "\t End Pos (x,y,z) : (" << g4_part_end_pos_X->at(j) << ", " << g4_part_end_pos_Y->at(j) << ", " << g4_part_end_pos_Z->at(j)  << ")" << endl;
                */
            }

            // ************************ KAON LONG ************************************************************+
            if (g4_part_PDGcode->at(j) == 130){
                //cout << " -------- EVENT " << i_e << " -------------- " << endl;
                n_kl++;
                t_kl++;


                if (n_k0 == 0){
                    t_kl_nok0++;
                    cout << endl;
                    cout << "================ Kl WITHOUT K0 AT EVENT " << i_e << "======================" << endl;
                    cout << "kl process: " << g4_part_process->at(j) << endl;
                    cout << "\t kl ID: " << g4_part_trackID->at(j) << endl;
                    cout << "\t kl mother: " << g4_part_mother->at(j) << endl;
                    cout << "\t Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(j) << ", " << g4_part_start_pos_Y->at(j) << ", " << g4_part_start_pos_Z->at(j)  << ")" << endl;

                    /*
                    cout << "------------ MOTHER OPTION 1 (ID) --------------" << endl;
                    int i_m1 = g4_part_mother->at(j);
                    cout << "\t Mother1 PDG: " << g4_part_PDGcode->at(i_m1) << endl;
                    cout << "\t Mother1 End Pos (x,y,z): (" << g4_part_end_pos_X->at(i_m1) << ", " << g4_part_end_pos_Y->at(i_m1) << ", " << g4_part_end_pos_Z->at(i_m1)  << ")" << endl;
                    */

                    
                    //cout << "------------ MOTHER OPTION 2 (ID-1) --------------" << endl;
                    cout << "------------ MOTHER INFO --------------" << endl;
                    
                    cout << "Tamaño de vector g4: " << g4_part_mother->size() << endl;
                    cout << "Tamaño de vector gen: " << gen_part_mother->size() << endl;

                    int i_m2 = findPos(g4_part_trackID, g4_part_mother->at(j));
                    cout << "\t Mother2 PDG: " << g4_part_PDGcode->at(i_m2) << endl;
                    cout << "\t Mother2 End Pos (x,y,z): (" << g4_part_end_pos_X->at(i_m2) << ", " << g4_part_end_pos_Y->at(i_m2) << ", " << g4_part_end_pos_Z->at(i_m2)  << ")" << endl;
                    

                }
            }

            // *********************************************** KAON SHORT *********************************************************+
            if (g4_part_PDGcode->at(j) == 310){
                //cout << " -------- EVENT " << i_e << " -------------- " << endl;
                n_ks++;
                t_ks++;
                
                if (n_k0 == 0){
                    t_ks_nok0++;
                    cout << endl;
                    cout << endl;
                    cout << "================ Ks WITHOUT K0 AT EVENT " << i_e << "======================" << endl;
                    cout << "ks process: " << g4_part_process->at(j) << endl;
                    cout << "\t ks ID: " << g4_part_trackID->at(j) << endl;
                    cout << "\t ks mother: " << g4_part_mother->at(j) << endl;
                    cout << "\t Start Pos (x,y,z) : (" << g4_part_start_pos_X->at(j) << ", " << g4_part_start_pos_Y->at(j) << ", " << g4_part_start_pos_Z->at(j)  << ")" << endl;

                    /*
                    cout << "------------ MOTHER OPTION 1 (ID) --------------" << endl;
                    int i_m1 = g4_part_mother->at(j);
                    cout << "\t Mother1 PDG: " << g4_part_PDGcode->at(i_m1) << endl;
                    cout << "\t Mother1 End Pos (x,y,z): (" << g4_part_end_pos_X->at(i_m1) << ", " << g4_part_end_pos_Y->at(i_m1) << ", " << g4_part_end_pos_Z->at(i_m1)  << ")" << endl;
                    */
                    
                    
                    //cout << "------------ MOTHER OPTION 2 (ID-1) --------------" << endl;
                    cout << "------------ MOTHER INFO --------------" << endl;
                    int i_m2 = findPos(g4_part_trackID, g4_part_mother->at(j));;
                    cout << "\t Mother2 PDG: " << g4_part_PDGcode->at(i_m2) << endl;
                    cout << "\t Mother2 End Pos (x,y,z): (" << g4_part_end_pos_X->at(i_m2) << ", " << g4_part_end_pos_Y->at(i_m2) << ", " << g4_part_end_pos_Z->at(i_m2)  << ")" << endl;
                    

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

    cout << endl;
    cout << "Total ks " << t_ks << "      " << "Ks without primary k0: " << t_ks_nok0 << endl; 
    cout << "Total kl " << t_kl << "      " << "Kl without primary k0: " << t_kl_nok0 << endl;


}