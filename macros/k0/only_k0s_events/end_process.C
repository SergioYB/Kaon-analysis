#include "/home/sergioyb/root/kaon_analysis/macros/includes/tree_utils.cpp"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/Includes.h"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_event_tree.C"
#include "/home/sergioyb/root/kaon_analysis/macros/includes/read_subrun_tree.C"
#include <iostream>
#include <set>
#include <vector>
#include <string>



void end_process(){

    TTree *event_tree;
    TTree *subrun_tree;

    string data_dir = "/home/sergioyb/root/kaon_analysis/data/analysis_output_truth_k0_events.root";

    event_tree = (TTree*)read_event_tree(data_dir);
    //subrun_tree = (TTree*)read_subrun_tree();

    int n_events = event_tree->GetEntries();
    float size_g4; // size of the vector containing g4 particles of each event
    float size_gen;

    std::set<std::string> end_processes; //Stores end processes

    //Create canvas and histogram 
    TCanvas *c1 = new TCanvas("c1", "canvas1", 800, 600);
    TH1F *hist = new TH1F("hist", "", 2, 0.5 , 2.5); //x-> Decay or Inelastic

    //Loop over the number of events
    for(int i_e = 0; i_e < n_events; i_e++) {
        //Get tree i-th entry and the size of ALL particle at i-th event
        event_tree->GetEntry(i_e);
        size_g4 = g4_part_trackID->size();
        size_gen = gen_part_trackID->size();

        //Loop over gen particles to finds primary k0S produced by resonance decay
        for(int i_p = 0; i_p < size_g4; i_p++){
            if((g4_part_PDGcode->at(i_p) == 310) & (g4_part_process->at(i_p) == "primary")){
                
                end_processes.insert(g4_part_end_process->at(i_p));

                if (g4_part_end_process->at(i_p) == "Decay"){
                    hist->Fill(1);
                } else if (g4_part_end_process->at(i_p) == "kaon0SInelastic"){
                    hist->Fill(2);
                } else {
                    cout << "THERE ARE OTHER END MODES" << endl;
                }
            }
        }
    }

    cout << "End processes: " << endl;
    for (const auto& proc : end_processes) {
        std::cout << proc << std::endl;
    }

    //Draw histogram
    hist->SetStats(0);

    hist->GetXaxis()->SetBinLabel(1, "Decay");
    hist->GetXaxis()->SetBinLabel(2, "Inelastic");

    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.07);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.05);

    hist->GetXaxis()->SetTitle("End mode");
    hist->GetYaxis()->SetTitle("Events");

    //hist->Scale(1e21/(3 * n_POT));

    hist->SetMinimum(0);

    //Add text
    TLatex latex;
    double n_Dec = hist->GetBinContent(1);
    double n_In = hist->GetBinContent(2);
    double totalK = hist->Integral();

    //Color settings

    hist->SetLineColor(kGreen);
    hist->SetLineWidth(3);

    hist->SetFillColor(kGreen-9);
    //hist->SetFillStyle(1001); 

    hist->Draw("HIST");

    latex.DrawLatex(0.9, n_Dec * 0.9, Form("%.2f%%", n_Dec/totalK*100)); //Draw text
    latex.DrawLatex(1.9, n_In * 1.2, Form("%.2f%%", n_In/totalK*100)); //Draw text

    c1->Update();

    cout << n_Dec << endl;
}