#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>



void sisters_k0_dis(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_BNB_single_100k_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    bool par_found;

    std::set<TString> topologies_found;
    std::map<TString, int> top_found_map;

    //std::set<std::string> current_products; //Stores products in k0 events
    TString current_topology;

    std::map<TString, int> n_par;
    std::map<TString, int> parPDG;

    //Quark counting for barion charge
    int q1;
    int q2;
    int q3;
    int B_charge;

    parPDG["K^{0}"] = 311;
    parPDG["K^{#pm}"] = 321;
    parPDG["#pi^{0}"] = 111;
    parPDG["#pi^{#pm}"] = 211;
    parPDG["#mu^{-}"] = 13;
    parPDG["p"] = 2212;
    parPDG["n"] = 2112;
    parPDG["#nu_{#mu}"] = 14;
    parPDG["#gamma"] = 22;
    parPDG["e"] = 11;
    parPDG["#nu_{e}"] = 12;
    parPDG["B"];
    parPDG["B^{0}"];
    parPDG["B^{#pm}"];

    n_par["K^{0}"];
    n_par["K^{#pm}"];
    n_par["#pi^{0}"];
    n_par["#pi^{#pm}"];
    n_par["#mu{-}"];
    n_par["p"];
    n_par["n"];
    n_par["#nu_{#mu}"];
    n_par["#gamma"];
    n_par["e"];
    n_par["#nu_{e}"];
    n_par["B"];
    n_par["B^{0}"];
    n_par["B^{#pm}"];

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *h1= new TH1F("h1", "", 5, 0.5 , 5.5); //B0
    TH1F *h2 = new TH1F("h2", "", 5, 0.5, 5.5); //B+-
    TH1F *h3 = new TH1F("h3", "", 5, 0.5, 5.5); //B+-

    int current_ntrace;

    bool kaon_found;

    /************************ RESONANCE DECAY ******************************/

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        current_ntrace = 0;

        kaon_found = false;

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((!kaon_found) & (g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (nu_interaction_mode == 2) & (nu_interaction_type == 1091)){

                //current_products = {}; //initialize products
                current_topology = TString(""); //initialize topology

                //initialize counting to 0
                for (auto& pair : n_par) {
                    pair.second = 0;
                }
                kaon_found = true;


                //Loop over g4 particles to find k0 sisters
                for (int j_p = 0; j_p < size_g4; j_p++){
                    par_found = false;
                    if ((g4_part_mother->at(j_p) == g4_part_mother->at(i_p)) & (g4_part_process->at(j_p) == "primary")){ //Both conditions equivalent?

                        if ((g4_part_PDGcode->at(j_p) == 2212) || (std::abs(g4_part_PDGcode->at(j_p)) == 211)){
                            current_ntrace++;
                        }  

                        //Loop over all particles in map 
                        for (const auto& pair : parPDG) {
                            if (std::abs(g4_part_PDGcode->at(j_p)) == pair.second){
                                n_par[pair.first]++;
                                par_found = true;
                                break;
                            }
                        } 
                        // Find other farticles 
                        if (!par_found & (g4_part_PDGcode->at(j_p) >= 2200 & g4_part_PDGcode->at(j_p) <= 6200)){
                            q1 = g4_part_PDGcode->at(j_p) / 1000;        // División entre 1000 para obtener la primera cifra
                            q2 = (g4_part_PDGcode->at(j_p) / 100) % 10;  // División entre 100 y módulo 10 para obtener la segunda cifra
                            q3 = (g4_part_PDGcode->at(j_p) / 10) % 10;
                            B_charge = (((q1+1) % 2 *3 - 1) + ((q2+1) % 2 *3 - 1) + ((q3+1) % 2 *3 - 1)) / 3;

                            //Distinguish charged baryon from neutral one
                            if (B_charge == 0){
                                //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                                n_par["B^{0}"]++;
                            } else {
                                //cout << g4_part_PDGcode->at(j_p) << "    " << B_charge << endl;
                                n_par["B^{#pm}"]++;
                            }
                            
                            //n_par["B"]++;
                            par_found = true;

                        } else if (!par_found){
                            cout << "Other particle PDG: " << g4_part_PDGcode->at(j_p) << endl;
                        }
                    }
                }

                //Fill histograms
                if (n_par["B^{0}"]){
                    h1->Fill(fmin(current_ntrace+1,4));
                } else if (n_par["B^{#pm}"]){
                    h2->Fill(fmin(current_ntrace+1,4));
                } else {
                    h3->Fill(fmin(current_ntrace+1,4));
                }

                //Add topology

                for (const auto& pair : n_par) {
                    if (pair.second){
                        current_topology = current_topology + TString(std::to_string(pair.second)) + pair.first;
                    }
                }

                //current_topology = current_topology  + TString(std::to_string(i_e));

                topologies_found.insert(current_topology);
                top_found_map[current_topology]++;
            }
        }
       
    }



    cout << "Topologies found: " << endl;
    for (const auto& pair : top_found_map) {
        cout << pair.first << "    " << pair.second << endl;
    }

    /*
    //Fill histogram considering most common topologies
    int i_h = 1;
    for (const auto& pair : top_found_map) {
        if (pair.second >= 2){
            h1->Fill(i_h,pair.second);
            h1->GetXaxis()->SetBinLabel(i_h, pair.first);
            i_h++;
        } else {
            h1->Fill(3,pair.second);
        }
    }
    */

    //h1->GetXaxis()->SetBinLabel(3, "No Baryons");

    /*
    //Fill considering B charge in different bins
    int i_h = 1;
    for (const auto& pair : top_found_map) {
        if (pair.first.Index("B^{0}") != -1){
            h1->Fill(1, pair.second);
        } else if (pair.first.Index("B^{#pm}") != -1){
            h1->Fill(2, pair.second);
        } else {
            h1->Fill(3, pair.second);
        }
    }
    */
    
        

    //Draw histograms
    
    //Color settings
    h1->SetLineColor(kGreen);
    h1->SetLineWidth(3);
    h1->SetFillColor(kGreen-9);

    h2->SetLineColor(kBlue);
    h2->SetLineWidth(3);
    h2->SetFillColor(kBlue-9);

    h3->SetLineColor(kRed);
    h3->SetLineWidth(3);
    h3->SetFillColor(kRed-9);

    //Create THStack histogram
    THStack *hs = new THStack("hs", "DIS CC;Primary traces;Events");

    hs->Add(h1);
    hs->Add(h2);
    hs->Add(h3);

    //Bin labels
    h1->GetXaxis()->SetBinLabel(1, "0");
    h1->GetXaxis()->SetBinLabel(2, "1");
    h1->GetXaxis()->SetBinLabel(3, "2");
    h1->GetXaxis()->SetBinLabel(4, "3+");

    hs->Draw("hist");
    
    hs->GetXaxis()->SetTitleSize(0.05);
    hs->GetYaxis()->SetTitleSize(0.05);
    hs->GetXaxis()->SetLabelSize(0.05);
    hs->GetYaxis()->SetLabelSize(0.05);

    

    //Add legend
    TLegend *legend = new TLegend(0.6, 0.65, 0.9, 0.9); 
    legend->AddEntry(h1, "Topology w B^{0}", "f");
    legend->AddEntry(h2, "Topology w B^{#pm}", "f");
    legend->AddEntry(h3, "Topology wo B", "f");
    legend->Draw();

    //Modify margin
    c1->SetLeftMargin(0.15);  
    c1->SetRightMargin(0.1);  
    c1->SetTopMargin(0.10);   
    c1->SetBottomMargin(0.15);

    c1->Update();

}