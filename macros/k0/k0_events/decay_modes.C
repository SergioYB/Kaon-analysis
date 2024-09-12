#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>

/*
*********** CREATE HISTOGRAM SHOWING K0S DECAY MODES **********************
*/

void decay_modes(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_k0_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    std::set<std::string> modes; //Stores decay modes

    std::string current_mode;


    bool children_found;

    //Create canvas and histogram 
    TCanvas *c = new TCanvas("c", "canvas", 800, 600);
    TH1F *hist = new TH1F("h", "", 3, 3.5 , 3.5); //x-> pi0pi0 of pi+pi.

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        children_found = false;


        //Show decay modes

        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 310) & (g4_part_process->at(i_p) == "primary") & (g4_part_end_process->at(i_p) == "Decay")){

                current_mode = ""; //initialize decay_mode

                //Loop over g4 particles to find k0 children
                for (int j_p = 0; j_p < size_g4; j_p++){
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (g4_part_process->at(j_p) == "Decay")){
                        if (g4_part_PDGcode->at(j_p) == 111){
                            current_mode = current_mode + "p0";
                            if (!children_found){
                                hist->Fill(1);
                            }

                        } else if (g4_part_PDGcode->at(j_p) == 211){
                            current_mode = current_mode + "p+";
                            if (!children_found){
                                hist->Fill(2);
                            }

                        } else if (g4_part_PDGcode->at(j_p) == -211){
                            current_mode = current_mode + "p-";
                            if (!children_found){
                                hist->Fill(2);
                            }

                        } else{
                            current_mode = current_mode + std::to_string(g4_part_PDGcode->at(j_p));
                            if (!children_found){
                                hist->Fill(3);
                            }
                        }
                        children_found = true;
                    }
                }

                modes.insert(current_mode);
            }
        }

        //Loop over gen particles to finds primary k0S produced by resonance decay
        /*for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 310) & (g4_part_process->at(i_p) == "primary") & (g4_part_end_process->at(i_p) == "Decay")){

                //Loop over g4 particles to find k0 children
                for (int j_p = 0; j_p < size_g4; j_p++){
                    if ((g4_part_mother->at(j_p) == g4_part_trackID->at(i_p)) & (!children_found)){
                        children_found = true;
                        if (g4_part_PDGcode->at(j_p) == 111){
                            hist->Fill(1);
                            modes.insert("p0");
                        } else if ((g4_part_PDGcode->at(j_p) == 211) || (g4_part_PDGcode->at(j_p) == -211)){
                            hist->Fill(2);
                            modes.insert("p+-");
                        } else{//} if (g4_part_process->at(j_p) == "Decay"){
                            for (int k_p = 0; k_p < size_g4; k_p++){
                                if ((g4_part_mother->at(k_p) == g4_part_trackID->at(i_p))){
                                    cout << endl;
                                    cout << endl << "========== EVENT " << i_e << " ================" << endl;
                                    cout << "\t Chldrn PDG: " << g4_part_PDGcode->at(k_p) << endl;
                                    cout << "\t  Chldrn process: " << g4_part_process->at(k_p) << endl;
                                    cout << "\t  Chldrn g4ID: " << g4_part_trackID->at(k_p) << endl;
                                    cout << "\t  Chldrn mother: " << g4_part_mother->at(k_p) << endl;
                                    cout << "\t  K0 ID; " << g4_part_trackID->at(i_p) << endl;
                                }
                            }
                            modes.insert(std::to_string(g4_part_PDGcode->at(j_p)));
                        }
                    }
                }
            }
        }*/
    }

    cout << "Decay modes: " << endl;
    for (const auto& proc : modes) {
        cout << "\t" << proc << endl;
    }

    //Draw histogram
    hist->SetStats(0);

    hist->GetXaxis()->SetBinLabel(1, "#pi^{0} #pi^{0}");
    hist->GetXaxis()->SetBinLabel(2, "#pi^{+} #pi^{-}");

    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.07);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.05);

    hist->GetXaxis()->SetTitle("Decay mode");
    hist->GetYaxis()->SetTitle("Events");

    //hist->Scale(1e21/(3 * n_POT));

    hist->SetMinimum(0);

    //Add text
    TLatex latex;
    double n_p0 = hist->GetBinContent(1);
    double n_ppm = hist->GetBinContent(2);
    double totalK = hist->Integral();

    //Color settings

    hist->SetLineColor(kGreen);
    hist->SetLineWidth(3);

    hist->SetFillColor(kGreen-9);
    //hist->SetFillStyle(1001); 

    hist->Draw("HIST");

    latex.DrawLatex(0.9, n_p0 * 1.1, Form("%.2f%%", n_p0/totalK*100)); //Draw text
    latex.DrawLatex(1.9, n_ppm * 0.9, Form("%.2f%%", n_ppm/totalK*100)); //Draw text

    c->Update();

    cout << n_p0 + n_ppm << endl;
}