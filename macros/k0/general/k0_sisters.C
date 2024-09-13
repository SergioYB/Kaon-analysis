#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>



void k0_sisters(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    std::set<std::string> topologies_found;
    std::map<std::string, int> top_found_map;

    std::set<std::string> current_products; //Stores products in k0 events
    std::string current_topology;


    //Create canvas and histogram 
    //TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    //TH1F *hist = new TH1F("h", "", 3, 0.5 , 3.5); //x-> pi0pi0 of pi+pi.

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        std::map<std::string, int> n_par;
        std::map<std::string, int> parPDG;

        parPDG["k0"] = 311;
        parPDG["kpm"] = 321;
        parPDG["pi0"] = 111;
        parPDG["pipm"] = 211;
        parPDG["mu"] = 13;
        parPDG["p"] = 2212;
        parPDG["n"] = 2112;
        parPDG["B"];

        n_par["k0"];
        n_par["kpm"];
        n_par["pi0"];
        n_par["pipm"];
        n_par["mu"];
        n_par["p"];
        n_par["n"];
        n_par["B"];

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (nu_interaction_mode == 1)){

                //current_products = {}; //initialize products
                current_topology = ""; //initialize topology

                //initialize counting to 0
                for (auto& pair : n_par) {
                    pair.second = 0;
                }

                //Loop over g4 particles to find k0 sisters (same mother and stable final state (1))
                for (int j_p = 0; j_p < size_g4; j_p++){
                    if (g4_part_mother->at(j_p) == g4_part_mother->at(i_p)){
                        //Loop over all particles in map   
                        for (const auto& pair : parPDG) {
                            if (std::abs(g4_part_PDGcode->at(j_p)) == pair.second){
                                n_par[pair.first]++;
                                //current_products.insert(pair.first);
                                break;
                            }
                        } //******************** añadir un bool para detectar otras particulas **************************+
                    }
                }

                //Add topology

                for (const auto& pair : n_par) {
                    if (pair.second){
                        current_topology = current_topology + std::to_string(pair.second) + pair.first;
                    }
                }

                topologies_found.insert(current_topology);
                top_found_map[current_topology]++;
            }
        }
       
    }

    cout << "RESONANCE DECAY" << endl << endl;

    cout << "Topologies found: " << endl;
    for (const auto& pair : top_found_map) {
        cout << pair.first << "    " << pair.second << endl;
    }

}



/*
                        if (g4_part_PDGcode->at(j_p) == 311){ //find k0
                            current_product.insert("k0");
                            n_k0++;

                        } else if (g4_part_PDGcode->at(j_p) == -311){ //find anti-k0
                            current_product.insert("ak0");
                            n_ak0++;

                        } else if ((g4_part_PDGcode->at(j_p) == 321) || (g4_part_PDGcode->at(j_p) == -321)){
                            current_product.insert("pipm");
                            n_pipm++;

                        }else if ((g4_part_PDGcode->at(j_p) == 211) || (g4_part_PDGcode->at(j_p) == -211)){
                            current_product.insert("pipm");
                            n_pipm++;

                        } else if (g4_part_PDGcode->at(j_p) == 111){
                            current_product.insert("pi0");
                            n_pi0++;

                        } else if (g4_part_PDGcode->at(j_p) == 2212){
                            current_product.insert("p");
                            n_p++;

                        } else if (g4_part_PDGcode->at(j_p) == 2112){
                            current_product.insert("n");
                            n_n++;

                        } else{

                        }*/