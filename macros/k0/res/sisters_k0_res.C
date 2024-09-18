#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>



void sisters_k0_res(){

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
    //parPDG["#gamma"] = 22;
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
    //n_par["#gamma"];
    n_par["e"];
    n_par["#nu_{e}"];
    n_par["B"];
    n_par["B^{0}"];
    n_par["B^{#pm}"];


    //Create canvas and histogram 
    TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    TH1F *hist = new TH1F("h", "", 3, 0.5 , 3.5); //x-> pi0pi0 of pi+pi.


    /************************ RESONANCE DECAY ******************************/

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 311) & (g4_part_process->at(i_p) == "primary") & (nu_interaction_mode == 1)){

                //current_products = {}; //initialize products
                current_topology = TString(""); //initialize topology

                //initialize counting to 0
                for (auto& pair : n_par) {
                    pair.second = 0;
                }

                
                //Loop over g4 particles to find k0 sisters (same mother and stable final state (1))
                for (int j_p = 0; j_p < size_g4; j_p++){
                    par_found = false;
                    if ((g4_part_mother->at(j_p) == g4_part_mother->at(i_p)) & (g4_part_process->at(j_p) == "primary")){ //Both conditions equivalent?
                        //Loop over all particles in map   
                        for (const auto& pair : parPDG) {
                            if (std::abs(g4_part_PDGcode->at(j_p)) == pair.second){
                                n_par[pair.first]++;
                                par_found = true;
                                break;
                            }
                        } 
                        if (g4_part_PDGcode->at(j_p) == 22){
                            cout << "foton con madre " << g4_part_mother->at(j_p) << endl;
                            cout << "foton energy " << g4_part_E0->at(j_p) << endl; 
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

                //Add topology

                for (const auto& pair : n_par) {
                    if (pair.second){
                        current_topology = current_topology + TString(std::to_string(pair.second)) + pair.first;
                    }
                }

                topologies_found.insert(current_topology);
                top_found_map[current_topology]++;
            }
        }
       
    }

    //Fill histograms
    cout << endl << "RESONANCE DECAY" << endl;

    cout << "Topologies found: " << endl;

    int i_h = 1;
    for (const auto& pair : top_found_map) {
        cout << pair.first << "    " << pair.second << endl;
        hist->Fill(i_h, pair.second);
        i_h++;
    }

    //Draw histogram
    hist->SetStats(0);

    /*
    hist->GetXaxis()->SetBinLabel(1, "1K^{0}1B1#nu_{#mu}");
    hist->GetXaxis()->SetBinLabel(2, "1K^{0}1B1#mu^{-}");
    hist->GetXaxis()->SetBinLabel(3, "1K^{0}1B1#nu_{#mu}1#gamma");
    */

    hist->GetXaxis()->SetBinLabel(1, "1K^{0}1B^{+}1#mu^{-}");
    hist->GetXaxis()->SetBinLabel(2, "1K^{0}1B^{+}1#nu_{#mu}");
    hist->GetXaxis()->SetBinLabel(3, "1K^{0}1B^{0}1#nu_{#mu}");


    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.06);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.06);

    hist->GetXaxis()->SetTitle("Topology");
    hist->GetYaxis()->SetTitle("Events");

    //Color settings
    hist->SetLineColor(kGreen);
    hist->SetLineWidth(3);
    hist->SetFillColor(kGreen-9);

    //Modify margin
    c->SetLeftMargin(0.15);  
    c->SetRightMargin(0.1);  
    c->SetTopMargin(0.10);   
    c->SetBottomMargin(0.15);
    

    //Add text
    TLatex latex;
    TLatex latex_white;

    latex_white.SetTextColor(kWhite);

    hist->Draw("HIST");

    //latex.DrawLatex(hist->GetXaxis()->GetBinCenter(1), hist->GetYaxis()->GetBinCenter(1), Form("%.0f", hist->GetBinContent(1,1)));
    //latex.DrawLatex(hist->GetXaxis()->GetBinCenter(2), hist->GetYaxis()->GetBinCenter(1), Form("%.0f", hist->GetBinContent(2,1)));
    //latex_white.DrawLatex(hist->GetXaxis()->GetBinCenter(2), hist->GetYaxis()->GetBinCenter(2), Form("%.0f", hist->GetBinContent(2,2)));
    //latex_white.DrawLatex(hist->GetXaxis()->GetBinCenter(1), hist->GetYaxis()->GetBinCenter(2), Form("%.0f", hist->GetBinContent(1,2)));

    c->Update();

}